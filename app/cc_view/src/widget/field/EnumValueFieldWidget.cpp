//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "EnumValueFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <limits>

namespace cc_tools_qt
{

namespace
{

const QString InvalidValueComboText("???");
const int EnumValuesStartIndex = 2;

}  // namespace

EnumValueFieldWidget::EnumValueFieldWidget(
    FieldPtr&& fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);

    assert(m_ui.m_serValueLineEdit != nullptr);
    setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_fieldPtr->minWidth(), m_fieldPtr->maxWidth());

    populateComboBox();

    commonConstruct();

    refresh();

    connect(
        m_ui.m_valueComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &EnumValueFieldWidget::valueUpdated);

    connect(
        m_ui.m_serValueLineEdit, &QLineEdit::textEdited,
        this, &EnumValueFieldWidget::serialisedValueUpdated);
}

EnumValueFieldWidget::~EnumValueFieldWidget() noexcept = default;

ToolsField& EnumValueFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void EnumValueFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_fieldPtr->getSerialisedString());

    bool valid = m_fieldPtr->valid();
    auto comboIdx = m_ui.m_valueComboBox->currentIndex();

    auto comboRetrieveValueFunc =
        [this](int idx) -> UnderlyingType
        {
            auto comboValueVar = m_ui.m_valueComboBox->itemData(idx);
            assert(comboValueVar.isValid());
            assert(comboValueVar.canConvert<UnderlyingType>());
            return comboValueVar.value<UnderlyingType>();
        };

    do {
        if ((!valid) && (comboIdx < m_idxOffset)) {
            break;
        }

        auto value = m_fieldPtr->getValue();
        auto comboValue = comboRetrieveValueFunc(m_ui.m_valueComboBox->currentIndex());
        if (value == comboValue) {
            break;
        }

        bool foundMatch = false;
        auto itemsCount = m_ui.m_valueComboBox->count();
        for (auto idx = m_idxOffset; idx < itemsCount; ++idx) {
            comboValue = comboRetrieveValueFunc(idx);
            if (value == comboValue) {
                m_ui.m_valueComboBox->setCurrentIndex(idx);
                foundMatch = true;
                break;
            }
        }

        if (foundMatch) {
            break;
        }

        m_ui.m_valueComboBox->setCurrentIndex(0);

    } while (false);

    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void EnumValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void EnumValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    handleNumericSerialisedValueUpdate(value, *m_fieldPtr);
}

void EnumValueFieldWidget::valueUpdated(int idx)
{
    if ((!m_fieldPtr->valid()) && (idx < m_idxOffset)) {
        return;
    }

    if (isEditEnabled()) {
        auto valueVar = m_ui.m_valueComboBox->currentData();
        assert(valueVar.isValid());
        assert(valueVar.canConvert<UnderlyingType>());
        auto value = valueVar.value<UnderlyingType>();
        if (value == m_fieldPtr->getValue()) {
            return;
        }

        assert(isEditEnabled());
        m_fieldPtr->setValue(value);
        if (!m_fieldPtr->canWrite()) {
            m_fieldPtr->reset();
            assert(m_fieldPtr->canWrite());
        }
        emitFieldUpdated();
    }
    refresh();
}

void EnumValueFieldWidget::populateComboBox()
{
    auto& values = m_fieldPtr->values();
    auto maxValue = std::numeric_limits<long long int>::min();
    for (auto& val : values) {
        m_ui.m_valueComboBox->addItem(val.first, val.second);
        maxValue = std::max(maxValue, val.second);
    }

    auto invValue = std::numeric_limits<long long int>::min();
    if (maxValue < std::numeric_limits<long long int>::max()) {
        invValue = maxValue + 1;
    }

    auto len = m_fieldPtr->length();
    auto shift = len * std::numeric_limits<std::uint8_t>::digits;
    auto maxAllowedValue = std::numeric_limits<UnderlyingType>::max();
    if (len < sizeof(UnderlyingType)) {
        maxAllowedValue = static_cast<UnderlyingType>((static_cast<long long unsigned>(1) << shift) - 1);
    }

    do {
        if ((invValue < 0) && (maxValue < maxAllowedValue))  {
            invValue = maxAllowedValue;
            break;
        }

        if ((0 <= invValue) && (invValue <= maxAllowedValue)) {
            break;
        }

        std::vector<UnderlyingType> storedValues;
        for (auto idx = 0; idx < m_ui.m_valueComboBox->count(); ++idx) {
            storedValues.push_back(m_ui.m_valueComboBox->itemData(idx).value<UnderlyingType>());
        }

        invValue = maxAllowedValue - 1;
        while (0 < invValue) {
            if (std::none_of(
                storedValues.begin(), storedValues.end(),
                [invValue](UnderlyingType val) -> bool
                {
                    return val == invValue;
                })) {

                break;
            }

            --invValue;
        }

    } while (false);

    m_idxOffset = 0;
    if (0 <= invValue) {
        m_ui.m_valueComboBox->insertItem(0, InvalidValueComboText, QVariant(invValue));
        m_ui.m_valueComboBox->insertSeparator(1);
        m_idxOffset = EnumValuesStartIndex;
    }
}

}  // namespace cc_tools_qt



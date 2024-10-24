//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "BitmaskValueFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <type_traits>

#include <QtWidgets/QCheckBox>

#include "cc_tools_qt/property/field.h"

namespace cc_tools_qt
{

BitmaskValueFieldWidget::BitmaskValueFieldWidget(
    FieldPtr&& fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr)),
    m_checkboxes(m_fieldPtr->bitIdxLimit())
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);

    assert(m_ui.m_serValueLineEdit != nullptr);
    setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_fieldPtr->width());

    prepareCheckboxes();

    refresh();

    connect(m_ui.m_serValueLineEdit, SIGNAL(textEdited(const QString&)),
            this, SLOT(serialisedValueUpdated(const QString&)));
}

BitmaskValueFieldWidget::~BitmaskValueFieldWidget() noexcept = default;

ToolsField& BitmaskValueFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void BitmaskValueFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_fieldPtr->getSerialisedString());

    auto bitIdxLimit = m_fieldPtr->bitIdxLimit();
    assert(bitIdxLimit == m_checkboxes.size());
    for (auto idx = 0U; idx < bitIdxLimit; ++idx) {
        auto* checkbox = m_checkboxes[idx];
        if (checkbox == nullptr) {
            continue;
        }

        bool showedBitValue = checkbox->checkState() != 0;
        bool actualBitValue = m_fieldPtr->bitValue(idx);
        if (showedBitValue != actualBitValue) {
            Qt::CheckState state = Qt::Unchecked;
            if (actualBitValue) {
                state = Qt::Checked;
            }
            checkbox->setCheckState(state);
        }
    }

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void BitmaskValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void BitmaskValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    handleNumericSerialisedValueUpdate(value, *m_fieldPtr);
}

void BitmaskValueFieldWidget::checkBoxUpdated(int value)
{
    bool updated = false;
    if (isEditEnabled()) {
        auto* checkbox = sender();
        auto iter = std::find(m_checkboxes.begin(), m_checkboxes.end(), checkbox);
        if (iter == m_checkboxes.end()) {
            [[maybe_unused]] static constexpr bool Should_not_happen = false;
            assert(Should_not_happen);    
            return;
        }
        auto idx = static_cast<unsigned>(std::distance(m_checkboxes.begin(), iter));
        m_fieldPtr->setBitValue(idx, value != 0);
        updated = true;
        if (!m_fieldPtr->canWrite()) {
            m_fieldPtr->reset();
            assert(m_fieldPtr->canWrite());
        }
    }

    refresh();
    if (updated) {
        emitFieldUpdated();
    }
}

void BitmaskValueFieldWidget::prepareCheckboxes()
{
    auto& bitNamesList = m_fieldPtr->bits();

    m_checkboxes.clear();

    auto count = std::min(static_cast<unsigned>(bitNamesList.size()), m_fieldPtr->bitIdxLimit());
    m_checkboxes.resize(m_fieldPtr->bitIdxLimit());

    for (unsigned idx = 0; idx < count; ++idx) {
        auto name = bitNamesList[static_cast<int>(idx)];
        if (name.isEmpty()) {
            continue;
        }

        auto* checkbox = new QCheckBox(name);
        m_ui.m_checkboxesLayout->addWidget(checkbox);
        m_checkboxes[idx] = checkbox;

        connect(checkbox, SIGNAL(stateChanged(int)),
                this, SLOT(checkBoxUpdated(int)));
    }
}

}  // namespace cc_tools_qt



//
// Copyright 2017 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "LongLongIntValueFieldWidget.h"

#include "SpecialValueWidget.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

namespace cc_tools_qt
{

LongLongIntValueFieldWidget::LongLongIntValueFieldWidget(
    FieldPtr fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);
    m_ui.m_valueLineEdit->setText(QString("%1").arg(m_fieldPtr->getDisplayValue()));
    createSpecialsWidget(m_fieldPtr->specials());

    assert(m_ui.m_serValueLineEdit != nullptr);
    setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_fieldPtr->minWidth(), m_fieldPtr->maxWidth());

    commonConstruct();

    connect(
        m_ui.m_valueLineEdit, &QLineEdit::textEdited,
        this, &LongLongIntValueFieldWidget::valueUpdated);

    connect(
        m_ui.m_serValueLineEdit, &QLineEdit::textEdited,
        this, &LongLongIntValueFieldWidget::serialisedValueUpdated);

    refresh();
}

LongLongIntValueFieldWidget::~LongLongIntValueFieldWidget() noexcept = default;

ToolsField& LongLongIntValueFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void LongLongIntValueFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_fieldPtr->getSerialisedString());

    auto value = m_fieldPtr->getDisplayValue();
    assert(m_ui.m_valueLineEdit);
    if (getDisplayedValue(m_ui.m_valueLineEdit->text()) != value) {
        m_ui.m_valueLineEdit->setText(QString("%1").arg(value));
    }

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);

    if (m_specialsWidget != nullptr) {
        m_specialsWidget->setIntValue(m_fieldPtr->getValue());
    }
}

void LongLongIntValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_valueLineEdit->setReadOnly(readonly);
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void LongLongIntValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    handleNumericSerialisedValueUpdate(value, *m_fieldPtr);
}

void LongLongIntValueFieldWidget::valueUpdated(const QString& value)
{
    auto adjustedValue = getDisplayedValue(value);
    if (adjustedValue == m_fieldPtr->getDisplayValue()) {
        return;
    }

    assert(isEditEnabled());
    m_fieldPtr->setDisplayValue(adjustedValue);
    assert(m_fieldPtr->getDisplayValue() == adjustedValue);
    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->reset();
        assert(m_fieldPtr->canWrite());
    }
    refresh();
    emitFieldUpdated();
}

void LongLongIntValueFieldWidget::specialSelected(long long value)
{
    if (!isEditEnabled()) {
        refresh();
        return;
    }

    m_fieldPtr->setValue(value);
    refresh();
}

LongLongIntValueFieldWidget::DisplayedType
LongLongIntValueFieldWidget::getDisplayedValue(const QString& value)
{
    if ((value.isEmpty()) || (value == " ") || (value == "-") || (value == "+")) {
        return 0;
    }

    bool ok = false;
    auto val = value.toLongLong(&ok);
    if (ok) {
        return val;
    }

    static const int MinLength = std::numeric_limits<qlonglong>::digits10;
    assert(MinLength < value.size());
    QString valueCpy(value);
    while ((!ok) && (MinLength < valueCpy.size())) {
        valueCpy.resize(valueCpy.size() - 1);
        val = value.toLongLong(&ok);
    }

    return val;
}

bool LongLongIntValueFieldWidget::createSpecialsWidget(const SpecialsList& specials)
{
    delete m_specialsWidget;
    if (specials.empty()) {
        m_specialsWidget = nullptr;
        return false;
    }

    m_specialsWidget = new SpecialValueWidget(specials);
    connect(
        m_specialsWidget, &SpecialValueWidget::sigIntValueChanged,
        this, &LongLongIntValueFieldWidget::specialSelected);

    connect(
        m_specialsWidget, &SpecialValueWidget::sigRefreshReq,
        this, &LongLongIntValueFieldWidget::refresh);

    m_ui.m_valueWidgetLayout->insertWidget(m_ui.m_valueWidgetLayout->count(), m_specialsWidget);
    return true;
}

}  // namespace cc_tools_qt


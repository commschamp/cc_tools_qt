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

#include "FloatValueFieldWidget.h"

#include "SpecialValueWidget.h"

#include <QtCore/QTimer>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

namespace cc_tools_qt
{

namespace
{

const int DefaultDecimals = 6;

enum ValueType
{
    ValueType_val,
    ValueType_nan,
    ValueType_inf,
    ValueType_minusInf,
    ValueType_numOfValues
};

}  // namespace

FloatValueFieldWidget::FloatValueFieldWidget(
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

    assert(m_ui.m_serValueLineEdit != nullptr);
    setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_fieldPtr->minWidth(), m_fieldPtr->maxWidth());

    auto decimals = m_fieldPtr->decimals();
    if (decimals == 0) {
        decimals = DefaultDecimals;
    }
    m_ui.m_valueSpinBox->setDecimals(decimals);

    createSpecialsWidget(m_fieldPtr->specials());

    commonConstruct();

    refresh();

    connect(
        m_ui.m_valueSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
        this, &FloatValueFieldWidget::valueUpdated);

    connect(
        m_ui.m_serValueLineEdit, &QLineEdit::textEdited,
        this, &FloatValueFieldWidget::serialisedValueUpdated);

    connect(
        m_ui.m_typeComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &FloatValueFieldWidget::typeUpdated);
}

FloatValueFieldWidget::~FloatValueFieldWidget() noexcept = default;

ToolsField& FloatValueFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void FloatValueFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_fieldPtr->getSerialisedString());

    updateType();

    if (getTypeIndex() == ValueType_val) {
        updateSpinBoxValueRange();
        auto value = m_fieldPtr->getValue();
        assert(m_ui.m_valueSpinBox);
        if (m_ui.m_valueSpinBox->value() != value) {
            m_ui.m_valueSpinBox->setValue(value);
        }

        m_oldValue = m_fieldPtr->getValue();
    }

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);

    if (m_specialsWidget != nullptr) {
        m_specialsWidget->setFpValue(m_fieldPtr->getValue(), m_fieldPtr->getEpsilon());
    }

}

void FloatValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_valueSpinBox->setReadOnly(readonly);
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void FloatValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    handleNumericSerialisedValueUpdate(value, *m_fieldPtr);
}

void FloatValueFieldWidget::valueUpdated(double value)
{
    if ((std::isnan(value) && std::isnan(m_fieldPtr->getValue())) ||
        (value == m_fieldPtr->getValue())) {
        return;
    }

    assert(isEditEnabled());
    m_fieldPtr->setValue(value);
    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->reset();
    }
    refresh();
    emitFieldUpdated();
}

void FloatValueFieldWidget::typeUpdated(int value)
{
    assert(value < ValueType_numOfValues);
    bool updated = false;
    do {
        if (!isEditEnabled()) {
            break;
        }

        if (getTypeIndex() == value) {
            break;
        }

        updated = true;
        if (value == ValueType_nan) {
            m_fieldPtr->setNan();
            break;
        }

        if (value == ValueType_inf) {
            m_fieldPtr->setInf();
            break;
        }

        if (value == ValueType_minusInf) {
            m_fieldPtr->setMinusInf();
            break;
        }

        m_fieldPtr->setValue(m_oldValue);
    } while (false);

    refresh();

    if (updated) {
        emitFieldUpdated();
    }
}

void FloatValueFieldWidget::specialSelected(double value)
{
    if (!isEditEnabled()) {
        refresh();
        return;
    }

    valueUpdated(value);
}

void FloatValueFieldWidget::updateSpinBoxValueRange()
{
    auto value = m_fieldPtr->getValue();
    auto maxValue = value * 100;
    auto minValue = -value;
    do {
        if (0.0 < value) {
            break;
        }

        if (value < 0.0) {
            std::swap(maxValue, minValue);
            break;
        }

        maxValue = 10.0;
        minValue = -10.0;
    } while (false);

    m_ui.m_valueSpinBox->blockSignals(true);
    m_ui.m_valueSpinBox->setRange(minValue, maxValue);
    m_ui.m_valueSpinBox->blockSignals(false);
}

void FloatValueFieldWidget::updateType()
{
    int idx = getTypeIndex();
    bool valueVisible = (idx == ValueType_val);
    m_ui.m_valueSpinBox->setVisible(valueVisible);

    m_ui.m_typeComboBox->blockSignals(true);
    m_ui.m_typeComboBox->setCurrentIndex(idx);
    m_ui.m_typeComboBox->blockSignals(false);
}

int FloatValueFieldWidget::getTypeIndex()
{
    if (m_fieldPtr->isNan()) {
        return ValueType_nan;
    }

    if (m_fieldPtr->isInf()) {
        return ValueType_inf;
    }

    if (m_fieldPtr->isMinusInf()) {
        return ValueType_minusInf;
    }

    return ValueType_val;
}

bool FloatValueFieldWidget::createSpecialsWidget(const SpecialsList& specials)
{
    delete m_specialsWidget;
    if (specials.empty()) {
        m_specialsWidget = nullptr;
        return false;
    }

    m_specialsWidget = new SpecialValueWidget(specials);
    connect(
        m_specialsWidget, &SpecialValueWidget::sigFpValueChanged,
        this, &FloatValueFieldWidget::specialSelected);

    connect(
        m_specialsWidget, &SpecialValueWidget::sigRefreshReq,
        this, &FloatValueFieldWidget::refresh);

    m_ui.m_valueWidgetLayout->insertWidget(m_ui.m_valueWidgetLayout->count() - 1, m_specialsWidget);
    return true;
}

}  // namespace cc_tools_qt



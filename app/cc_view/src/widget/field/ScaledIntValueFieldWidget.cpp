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

#include "ScaledIntValueFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <cmath>

#include "SpecialValueWidget.h"

namespace cc_tools_qt
{

ScaledIntValueFieldWidget::ScaledIntValueFieldWidget(
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
    createSpecialsWidget(m_fieldPtr->specials());

    m_ui.m_valueSpinBox->setRange(
        m_fieldPtr->scaleValue(m_fieldPtr->minValue()),
        m_fieldPtr->scaleValue(m_fieldPtr->maxValue()));

    auto decimals = m_fieldPtr->scaledDecimals();
    if (0 < decimals) {
        m_ui.m_valueSpinBox->setDecimals(decimals);
    } 

    commonConstruct();

    refresh();

    connect(m_ui.m_valueSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(valueUpdated(double)));

    connect(m_ui.m_serValueLineEdit, SIGNAL(textEdited(const QString&)),
            this, SLOT(serialisedValueUpdated(const QString&)));
}

ScaledIntValueFieldWidget::~ScaledIntValueFieldWidget() noexcept = default;

ToolsField& ScaledIntValueFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void ScaledIntValueFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_fieldPtr->getSerialisedString());

    auto value = m_fieldPtr->getScaled();
    assert(m_ui.m_valueSpinBox);
    if (m_ui.m_valueSpinBox->value() != value) {
        m_ui.m_valueSpinBox->setValue(value);
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

void ScaledIntValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_valueSpinBox->setReadOnly(readonly);
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void ScaledIntValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    m_ui.m_valueSpinBox->blockSignals(true);
    handleNumericSerialisedValueUpdate(
        value,
        *m_fieldPtr,
        [this]() noexcept
        {
            m_ui.m_valueSpinBox->blockSignals(false);
        });
}

void ScaledIntValueFieldWidget::valueUpdated(double value)
{
    if (std::abs(value - m_fieldPtr->getScaled()) < std::numeric_limits<double>::epsilon()) {
        return;
    }

    assert(isEditEnabled());
    m_fieldPtr->setScaled(value);
    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->reset();
        assert(m_fieldPtr->canWrite());
    }

    refresh();
    emitFieldUpdated();
}

void ScaledIntValueFieldWidget::specialSelected(long long value)
{
    if (!isEditEnabled()) {
        refresh();
        return;
    }

    m_fieldPtr->setValue(static_cast<UnderlyingType>(value));
    refresh();
}

bool ScaledIntValueFieldWidget::createSpecialsWidget(const SpecialsList& specials)
{
    delete m_specialsWidget;
    if (specials.empty()) {
        m_specialsWidget = nullptr;
        return false;
    }

    m_specialsWidget = new SpecialValueWidget(specials);
    connect(
        m_specialsWidget, SIGNAL(sigIntValueChanged(long long)),
        this, SLOT(specialSelected(long long)));

    connect(
        m_specialsWidget, SIGNAL(sigRefreshReq()),
        this, SLOT(refresh()));

    m_ui.m_valueWidgetLayout->insertWidget(m_ui.m_valueWidgetLayout->count() - 1, m_specialsWidget);

    return true;
}

}  // namespace cc_tools_qt



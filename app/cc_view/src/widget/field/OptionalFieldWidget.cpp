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

#include "OptionalFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <type_traits>

#include <QtWidgets/QCheckBox>

#include "cc_tools_qt/property/field.h"

namespace cc_tools_qt
{

OptionalFieldWidget::OptionalFieldWidget(
    FieldPtr fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);

    commonConstruct();

    if (m_fieldPtr->getMode() == Mode::Tentative) {
        m_fieldPtr->setMode(Mode::Missing);
    }

    connect(m_ui.m_optCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(availabilityChanged(int)));
}

OptionalFieldWidget::~OptionalFieldWidget() noexcept = default;

void OptionalFieldWidget::setField(FieldWidget* fieldWidget)
{
    assert(m_field == nullptr);
    assert(fieldWidget != nullptr);
    m_field = fieldWidget;

    m_ui.m_layout->insertWidget(m_ui.m_layout->count() - 1, fieldWidget);

    refreshInternal();

    connect(
        fieldWidget, SIGNAL(sigFieldUpdated()),
        this, SLOT(fieldUpdated()));
}

ToolsField& OptionalFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void OptionalFieldWidget::refreshImpl()
{
    refreshInternal();
    refreshField();
}

void OptionalFieldWidget::editEnabledUpdatedImpl()
{
    assert(m_field != nullptr);
    m_field->setEditEnabled(isEditEnabled());
}

void OptionalFieldWidget::fieldUpdated()
{
    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->reset();
        assert(m_fieldPtr->canWrite());
        refreshField();
    }
    refreshInternal();
    emitFieldUpdated();
}

void OptionalFieldWidget::availabilityChanged(int state)
{
    Mode mode = Mode::Tentative;
    if (state == Qt::Unchecked) {
        mode = Mode::Missing;
    }
    else {
        mode = Mode::Exists;
    }

    if (mode == m_fieldPtr->getMode()) {
        return;
    }

    if (!isEditEnabled()) {
        refreshInternal();
        return;
    }

    m_fieldPtr->setMode(mode);
    refresh();
    emitFieldUpdated();
}

void OptionalFieldWidget::refreshInternal()
{
    assert(m_fieldPtr->canWrite());
    assert(m_field);
    auto mode = m_fieldPtr->getMode();
    if (mode == Mode::Exists) {
        m_ui.m_optCheckBox->setCheckState(Qt::Checked);
        m_ui.m_nameLabel->hide();
        hideSpacer();
        m_field->show();
    }
    else if (mode == Mode::Tentative) {
        m_ui.m_optCheckBox->setCheckState(Qt::Unchecked);
        m_ui.m_nameLabel->hide();
        hideSpacer();
        m_field->show();
    }
    else {
        assert(mode == Mode::Missing);
        m_ui.m_optCheckBox->setCheckState(Qt::Unchecked);
        m_ui.m_nameLabel->show();
        showSpacer();
        m_field->hide();
    }
}

void OptionalFieldWidget::refreshField()
{
    assert(m_field != nullptr);
    m_field->refresh();
}

void OptionalFieldWidget::showSpacer()
{
    m_ui.m_optSpacer->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void OptionalFieldWidget::hideSpacer()
{
    m_ui.m_optSpacer->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
}


}  // namespace cc_tools_qt



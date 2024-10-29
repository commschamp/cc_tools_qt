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

#include "UnknownValueFieldWidget.h"

#include <algorithm>
#include <cassert>

namespace cc_tools_qt
{

UnknownValueFieldWidget::UnknownValueFieldWidget(
    field::ToolsUnknownFieldPtr&& fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);

    commonConstruct();

    connect(m_ui.m_serValueLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(serialisedValueUpdated(const QString&)));

    refresh();
}

UnknownValueFieldWidget::~UnknownValueFieldWidget() noexcept = default;

ToolsField& UnknownValueFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void UnknownValueFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    auto curText = m_ui.m_serValueLineEdit->text();
    auto serString = m_fieldPtr->getSerialisedString();
    if (curText != serString) {

        assert(m_ui.m_serValueLineEdit != nullptr);
        setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_fieldPtr->width());
        m_ui.m_serValueLineEdit->setText(serString);
    }

    setFieldValid(m_fieldPtr->valid());
}

void UnknownValueFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
}

void UnknownValueFieldWidget::serialisedValueUpdated(const QString& value)
{
    assert(isEditEnabled());

    QString valueCopy(value);
    if ((valueCopy.size() & 0x1) != 0) {
        valueCopy.append(QChar('0'));
    }

    if (m_fieldPtr->setSerialisedString(valueCopy)) {
        assert(m_fieldPtr->canWrite());
        refresh();
        emitFieldUpdated();
    }
    else {
        setFieldValid(false);
    }
}

void UnknownValueFieldWidget::setFieldValid(bool valid)
{
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

}  // namespace cc_tools_qt



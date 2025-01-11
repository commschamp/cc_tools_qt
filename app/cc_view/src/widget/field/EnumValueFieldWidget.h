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


#pragma once

#include "cc_tools_qt/field/ToolsEnumField.h"
#include "FieldWidget.h"

#include "ui_EnumValueFieldWidget.h"

namespace cc_tools_qt
{

class EnumValueFieldWidget : public FieldWidget
{
    Q_OBJECT
    typedef FieldWidget Base;
public:
    using FieldPtr = field::ToolsEnumFieldPtr;

    explicit EnumValueFieldWidget(
        FieldPtr&& fieldPtr,
        QWidget* parentObj = nullptr);

    ~EnumValueFieldWidget() noexcept;

protected:
    virtual ToolsField& fieldImpl() override;
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;

private slots:
    void serialisedValueUpdated(const QString& value);
    void valueUpdated(int idx);

private:
    using FieldType = FieldPtr::element_type;
    using UnderlyingType = FieldType::UnderlyingType;

    void populateComboBox();

    Ui::EnumValueFieldWidget m_ui;
    FieldPtr m_fieldPtr;
    int m_idxOffset = 0;
};


}  // namespace cc_tools_qt



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

#include <vector>

#include "cc_tools_qt/field/ToolsBitfieldField.h"
#include "FieldWidget.h"

#include "ui_BitfieldFieldWidget.h"

namespace cc_tools_qt
{

class BitfieldFieldWidget : public FieldWidget
{
    Q_OBJECT
    typedef FieldWidget Base;
public:
    using FieldPtr = field::ToolsBitfieldFieldPtr;

    explicit BitfieldFieldWidget(
        FieldPtr&& fieldPtr,
        QWidget* parentObj = nullptr);

    ~BitfieldFieldWidget() noexcept;

    void addMemberField(FieldWidget* memberFieldWidget);

protected:
    virtual ToolsField& fieldImpl() override;
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;

private slots:
    void serialisedValueUpdated(const QString& value);
    void memberFieldUpdated();

private:
    void refreshInternal();
    void refreshMembers();

    Ui::BitfieldFieldWidget m_ui;
    FieldPtr m_fieldPtr;
    std::vector<FieldWidget*> m_members;
};


}  // namespace cc_tools_qt



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


#pragma once

#include "FieldWidget.h"
#include "ui_VariantFieldWidget.h"

#include "cc_tools_qt/field/ToolsVariantField.h"

#include <functional>

namespace cc_tools_qt
{

class VariantFieldWidget : public FieldWidget
{
    Q_OBJECT
    typedef FieldWidget Base;
public:
    using FieldPtr = field::ToolsVariantFieldPtr;
    using CreateMemberFieldWidgetFunc =
        std::function<FieldWidgetPtr (ToolsField&)>;

    explicit VariantFieldWidget(
        FieldPtr&& fieldPtr,
        CreateMemberFieldWidgetFunc&& func,
        QWidget* parentObj = nullptr);

    ~VariantFieldWidget() noexcept;

    void setMemberField(FieldWidget* memberFieldWidget);

protected:
    virtual ToolsField& fieldImpl() override;
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;

private slots:
    void memberFieldUpdated();
    void indexUpdated(int value);
    void memberComboUpdated(int value);

private:
    void refreshInternal();
    void refreshMember();
    void updateIndexDisplay();
    void updateIndexValue();
    void updateMemberCombo();
    void destroyMemberWidget();
    void createMemberWidget();
    void fillMemberCombo();

    Ui::VariantFieldWidget m_ui;
    FieldPtr m_fieldPtr;
    FieldWidget* m_member = nullptr;
    CreateMemberFieldWidgetFunc m_createFunc;
};


}  // namespace cc_tools_qt



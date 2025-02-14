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

#include "cc_tools_qt/field/ToolsOptionalField.h"
#include "FieldWidget.h"

#include "ui_OptionalFieldWidget.h"

namespace cc_tools_qt
{

class OptionalFieldWidget : public FieldWidget
{
    Q_OBJECT
    typedef FieldWidget Base;
public:
    using FieldPtr = field::ToolsOptionalFieldPtr;

    explicit OptionalFieldWidget(
        FieldPtr fieldPtr,
        QWidget* parentObj = nullptr);

    ~OptionalFieldWidget() noexcept;

    void setField(FieldWidget* fieldWidget);

protected:
    virtual ToolsField& fieldImpl() override;
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;

private slots:
    void fieldUpdated();
    void availabilityChanged(int state);

private:
    using FieldType = FieldPtr::element_type;
    typedef FieldType::Mode Mode;

    void refreshInternal();
    void refreshField();
    void showSpacer();
    void hideSpacer();

    Ui::OptionalFieldWidget m_ui;
    FieldPtr m_fieldPtr;
    FieldWidget* m_field = nullptr;
};


}  // namespace cc_tools_qt



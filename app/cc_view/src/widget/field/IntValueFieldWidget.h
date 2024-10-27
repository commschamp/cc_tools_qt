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


#pragma once

#include <memory>

#include "cc_tools_qt/field/ToolsIntField.h"
#include "FieldWidget.h"

namespace cc_tools_qt
{

class IntValueFieldWidget : public FieldWidget
{
    typedef FieldWidget Base;
public:
    using FieldPtr = field::ToolsIntFieldPtr;

    explicit IntValueFieldWidget(
        FieldPtr fieldPtr,
        QWidget* parentObj = nullptr);

    ~IntValueFieldWidget() noexcept;

protected:
    virtual ToolsField& fieldImpl() override;
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;

private:
    using WrapperType = FieldPtr::element_type;
    using UnderlyingType = WrapperType::UnderlyingType;

    FieldPtr m_fieldPtr;
    std::unique_ptr<FieldWidget> m_childWidget;
};


}  // namespace cc_tools_qt



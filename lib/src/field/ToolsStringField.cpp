//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/field/ToolsStringField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsStringField::ToolsStringField() = default;

ToolsStringField::~ToolsStringField() noexcept = default;

QString ToolsStringField::getValue() const
{
    return getValueImpl();
}

void ToolsStringField::setValue(const QString& val)
{
    setValueImpl(val);
}

int ToolsStringField::maxSize() const
{
    return maxSizeImpl();
}

ToolsStringField::ActPtr ToolsStringField::actClone()
{
    return ActPtr(static_cast<ToolsStringField*>(clone().release()));
}

void ToolsStringField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



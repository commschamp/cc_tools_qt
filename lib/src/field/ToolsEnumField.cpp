//
// Copyright 2016 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/field/ToolsEnumField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsEnumField::ToolsEnumField() {}

ToolsEnumField::~ToolsEnumField() noexcept = default;

const ToolsEnumField::ValueInfosList& ToolsEnumField::values() const
{
    return valuesImpl();
}

ToolsEnumField::ActPtr ToolsEnumField::actClone()
{
    return ActPtr(static_cast<ToolsEnumField*>(clone().release()));
}

void ToolsEnumField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt


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

#include "cc_tools_qt/field/ToolsArrayListField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsArrayListField::ToolsArrayListField() {}

ToolsArrayListField::~ToolsArrayListField() noexcept = default;

void ToolsArrayListField::addField()
{
    addFieldImpl();
}

void ToolsArrayListField::removeField(int idx)
{
    removeFieldImpl(idx);
}

unsigned ToolsArrayListField::size() const
{
    return sizeImpl();
}

bool ToolsArrayListField::hasFixedSize() const
{
    return hasFixedSizeImpl();
}

void ToolsArrayListField::adjustFixedSize()
{
    adjustFixedSizeImpl();
}

void ToolsArrayListField::refreshMembers()
{
    refreshMembersImpl();
}

ToolsArrayListField::ActPtr ToolsArrayListField::actClone()
{
    return ActPtr(static_cast<ToolsArrayListField*>(clone().release()));
}

void ToolsArrayListField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



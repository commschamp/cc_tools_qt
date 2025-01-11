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

#include "cc_tools_qt/field/ToolsOptionalField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsOptionalField::ToolsOptionalField() = default;
ToolsOptionalField::~ToolsOptionalField() noexcept = default;

ToolsOptionalField::Mode ToolsOptionalField::getMode() const
{
    return getModeImpl();
}

void ToolsOptionalField::setMode(Mode mode) 
{
    setModeImpl(mode);
}

bool ToolsOptionalField::hasField() const
{
    auto& members = getMembers();
    return !members.empty();
}

ToolsField& ToolsOptionalField::getField()
{
    auto& members = getMembers();
    assert(!members.empty());
    assert(hasField());
    return *members.front();
}

const ToolsField& ToolsOptionalField::getField() const
{
    auto& members = getMembers();
    assert(!members.empty());
    assert(hasField());
    return *members.front();
}

void ToolsOptionalField::setField(ToolsFieldPtr fieldPtr)
{
    assert(fieldPtr);
    Members mems;
    mems.push_back(std::move(fieldPtr));
    setMembers(std::move(mems));
}

ToolsOptionalField::ActPtr ToolsOptionalField::actClone()
{
    return ActPtr(static_cast<ToolsOptionalField*>(clone().release()));
}

void ToolsOptionalField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



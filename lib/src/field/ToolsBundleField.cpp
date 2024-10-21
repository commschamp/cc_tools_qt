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

#include "cc_tools_qt/field/ToolsBundleField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsBundleField::ToolsBundleField() {}

ToolsBundleField::~ToolsBundleField() noexcept = default;

ToolsBundleField::Members& ToolsBundleField::getMembers()
{
    return m_members;
}

const ToolsBundleField::Members& ToolsBundleField::getMembers() const
{
    return m_members;
}

void ToolsBundleField::setMembers(Members&& members)
{
    m_members = std::move(members);
} 

ToolsBundleField::ActPtr ToolsBundleField::clone()
{
    Members clonedMembers;
    clonedMembers.reserve(m_members.size());
    for (auto& mem : m_members) {
        clonedMembers.push_back(mem->upClone());
    }

    auto ptr = cloneImpl();
    ptr->setMembers(std::move(clonedMembers));
    return ptr;
}

void ToolsBundleField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



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

#include "cc_tools_qt/field/ToolsVariantField.h"

#include <cassert>

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsVariantField::ToolsVariantField() {}

ToolsVariantField::~ToolsVariantField() noexcept = default;

ToolsFieldPtr& ToolsVariantField::getCurrent()
{
    return m_current;
}

const ToolsFieldPtr& ToolsVariantField::getCurrent() const
{
    return m_current;
}

void ToolsVariantField::setCurrent(ToolsFieldPtr current)
{
    m_current = std::move(current);
}

void ToolsVariantField::updateCurrent()
{
    assert(m_createMemberCb);
    setCurrent(m_createMemberCb());
}

ToolsVariantField::ActPtr ToolsVariantField::clone()
{
    auto ptr = cloneImpl();
    ptr->m_createMemberCb = m_createMemberCb;
    if (m_current) {
        ptr->setCurrent(m_current->upClone());
    }

    return ptr;
}

QStringList ToolsVariantField::membersNames() const
{
    return membersNamesImpl();
}

int ToolsVariantField::getCurrentIndex() const
{
    return getCurrentIndexImpl();
}

void ToolsVariantField::setCurrentIndex(int index)
{
    setCurrentIndexImpl(index);
}

int ToolsVariantField::getMembersCount() const
{
    return getMembersCountImpl();
}

void ToolsVariantField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



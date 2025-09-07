//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsField.h"

#include <memory>
#include <vector>
#include <utility>

namespace cc_tools_qt
{

namespace field
{

class CC_TOOLS_API ToolsArrayListField : public ToolsField
{
public:

    using ActPtr = std::unique_ptr<ToolsArrayListField>;

    ToolsArrayListField();
    ToolsArrayListField(const ToolsArrayListField&) = delete;
    ToolsArrayListField& operator=(const ToolsArrayListField&) = delete;

    virtual ~ToolsArrayListField() noexcept;

    void addField();

    void removeField(int idx);

    unsigned size() const;

    bool hasFixedSize() const;

    void adjustFixedSize();

    void refreshMembers();

    ActPtr actClone();

protected:
    virtual void addFieldImpl() = 0;
    virtual void removeFieldImpl(int idx) = 0;
    virtual unsigned sizeImpl() const = 0;
    virtual bool hasFixedSizeImpl() const = 0;
    virtual void adjustFixedSizeImpl() = 0;
    virtual void refreshMembersImpl() = 0;

    void dispatchImpl(ToolsFieldHandler& handler);
};

using ToolsArrayListFieldPtr = ToolsArrayListField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt


//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "comms/field/Optional.h"

#include <memory>
#include <cassert>

namespace cc_tools_qt
{

namespace field
{

class CC_API ToolsOptionalField : public ToolsField
{
public:
    using ActPtr = std::unique_ptr<ToolsOptionalField>;
    using Mode = comms::field::OptionalMode;

    ToolsOptionalField();
    virtual ~ToolsOptionalField() noexcept;

    Mode getMode() const;

    void setMode(Mode mode);

    bool hasFieldWrapper() const;

    ToolsField& getFieldWrapper();

    const ToolsField& getFieldWrapper() const;

    void setFieldWrapper(ToolsFieldPtr fieldPtr);

    ActPtr actClone();

protected:
    virtual Mode getModeImpl() const = 0;
    virtual void setModeImpl(Mode mode) = 0;

    void dispatchImpl(ToolsFieldHandler& handler);
};

using ToolsOptionalFieldPtr = ToolsOptionalField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt



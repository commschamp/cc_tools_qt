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

#include "cc_tools_qt/ToolsField.h"

#include <memory>
#include <vector>

namespace cc_tools_qt
{

namespace field
{

class CC_TOOLS_API ToolsBundleField : public ToolsField
{
    using Base = ToolsField;
public:
    using ActPtr = std::unique_ptr<ToolsBundleField>;

    ToolsBundleField();
    ToolsBundleField(const ToolsBundleField&) =delete;
    ToolsBundleField& operator=(const ToolsBundleField&) =delete;

    virtual ~ToolsBundleField() noexcept;

    ActPtr actClone();

protected:
    void dispatchImpl(ToolsFieldHandler& handler);
};

using ToolsBundleFieldPtr = ToolsBundleField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt

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


#pragma once

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/version.h"

#include "cc_tools_qt/field/ToolsArrayListField.h"
#include "cc_tools_qt/field/ToolsBitfieldField.h"
#include "cc_tools_qt/field/ToolsBitmaskField.h"
#include "cc_tools_qt/field/ToolsBundleField.h"
#include "cc_tools_qt/field/ToolsEnumField.h"
#include "cc_tools_qt/field/ToolsFloatField.h"
#include "cc_tools_qt/field/ToolsIntField.h"
#include "cc_tools_qt/field/ToolsOptionalField.h"
#include "cc_tools_qt/field/ToolsRawDataField.h"
#include "cc_tools_qt/field/ToolsStringField.h"
#include "cc_tools_qt/field/ToolsUnknownField.h"
#include "cc_tools_qt/field/ToolsUnsignedLongField.h"
#include "cc_tools_qt/field/ToolsVariantField.h"
#include "cc_tools_qt/ToolsField.h"

namespace cc_tools_qt
{

class CC_TOOLS_API ToolsFieldHandler
{
public:
    virtual ~ToolsFieldHandler() noexcept = default;

    virtual void handle(field::ToolsArrayListField& field);
    virtual void handle(field::ToolsBitfieldField& field);
    virtual void handle(field::ToolsBitmaskField& field);
    virtual void handle(field::ToolsBundleField& field);
    virtual void handle(field::ToolsEnumField& field);
    virtual void handle(field::ToolsFloatField& field);
    virtual void handle(field::ToolsIntField& field);
    virtual void handle(field::ToolsOptionalField& field);
    virtual void handle(field::ToolsRawDataField& field);
    virtual void handle(field::ToolsStringField& field);
    virtual void handle(field::ToolsUnknownField& field);
    virtual void handle(field::ToolsUnsignedLongField& field);
    virtual void handle(field::ToolsVariantField& field);
    virtual void handle(ToolsField& field);

protected:
    ToolsFieldHandler() = default;
};

}  // namespace cc_tools_qt



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


#pragma once

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
#include "cc_tools_qt/field/ToolsUnsignedLongField.h"

#include "VariantWrapper.h"
#include "UnknownValueWrapper.h"

#include <tuple>

namespace cc_tools_qt
{

namespace field_wrapper
{

typedef std::tuple<
    cc_tools_qt::field::ToolsIntField,
    cc_tools_qt::field::ToolsUnsignedLongField,
    cc_tools_qt::field::ToolsBitmaskField,
    cc_tools_qt::field::ToolsEnumField,
    cc_tools_qt::field::ToolsStringField,
    cc_tools_qt::field::ToolsBitfieldField,
    cc_tools_qt::field::ToolsOptionalField,
    cc_tools_qt::field::ToolsBundleField,
    cc_tools_qt::field::ToolsRawDataField,
    cc_tools_qt::field::ToolsArrayListField,
    cc_tools_qt::field::ToolsFloatField,
    VariantWrapper,
    UnknownValueWrapper
> AllWrappers;

}  // namespace field_wrapper

}  // namespace cc_tools_qt



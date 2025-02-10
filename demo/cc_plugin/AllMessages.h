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

#include <tuple>
#include "DemoMessage.h"

#include "message/IntValues.h"
#include "message/EnumValues.h"
#include "message/BitmaskValues.h"
#include "message/Bitfields.h"
#include "message/Strings.h"
#include "message/Lists.h"
#include "message/Optionals.h"
#include "message/FloatValues.h"
#include "message/Variants.h"
#include "message/Bundles.h"

namespace demo
{

namespace cc_plugin
{

using AllMessages = std::tuple<
    cc_plugin::message::IntValues,
    cc_plugin::message::EnumValues,
    cc_plugin::message::BitmaskValues,
    cc_plugin::message::Bitfields,
    cc_plugin::message::Strings,
    cc_plugin::message::Lists,
    cc_plugin::message::Optionals,
    cc_plugin::message::FloatValues,
    cc_plugin::message::Variants,
    cc_plugin::message::Bundles
>;

static_assert(std::tuple_size<AllMessages>::value == MsgId_NumOfValues,
    "Some messages are missing");

}  // namespace cc_plugin

}  // namespace demo


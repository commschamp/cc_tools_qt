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
#include "demo/DemoMessage.h"
#include "demo/MsgId.h"

#include "demo/message/IntValues.h"
#include "demo/message/EnumValues.h"
#include "demo/message/BitmaskValues.h"
#include "demo/message/Bitfields.h"
#include "demo/message/Strings.h"
#include "demo/message/Lists.h"
#include "demo/message/Optionals.h"
#include "demo/message/FloatValues.h"
#include "demo/message/Variants.h"
#include "demo/message/Bundles.h"

namespace demo
{

template <typename TMsgBase, typename TOpt>
using AllMessages = std::tuple<
    demo::message::IntValues<TMsgBase, TOpt>,
    demo::message::EnumValues<TMsgBase, TOpt>,
    demo::message::BitmaskValues<TMsgBase, TOpt>,
    demo::message::Bitfields<TMsgBase, TOpt>,
    demo::message::Strings<TMsgBase, TOpt>,
    demo::message::Lists<TMsgBase, TOpt>,
    demo::message::Optionals<TMsgBase, TOpt>,
    demo::message::FloatValues<TMsgBase, TOpt>,
    demo::message::Variants<TMsgBase, TOpt>,
    demo::message::Bundles<TMsgBase, TOpt>
>;

}  // namespace demo


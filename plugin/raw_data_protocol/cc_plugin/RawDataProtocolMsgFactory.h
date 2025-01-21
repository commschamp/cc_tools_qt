//
// Copyright 2024 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "RawDataProtocolAllMessages.h"

#include "cc_tools_qt/ToolsMsgFactoryBase.h"


namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

namespace cc_plugin
{

using RawDataProtocolMsgFactory = cc_tools_qt::ToolsMsgFactoryBase<RawDataProtocolAllMessages>;

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt
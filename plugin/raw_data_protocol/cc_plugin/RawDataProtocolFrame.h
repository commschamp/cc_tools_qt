//
// Copyright 2024 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "RawDataProtocolMessage.h"
#include "RawDataProtocolMsgFactory.h"
#include "RawDataProtocolTransportMessage.h"

#include "raw_data_protocol/DataMessage.h"
#include "raw_data_protocol/Frame.h"

#include "cc_tools_qt/ToolsFrameBase.h"


namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

namespace cc_plugin
{

class RawDataProtocolFrame : public
    cc_tools_qt::ToolsFrameBase<
        RawDataProtocolMessage,
        raw_data_protocol::Frame<RawDataProtocolMessage::ProtInterface>,
        RawDataProtocolMsgFactory,
        RawDataProtocolTransportMessage
    >
{
};

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

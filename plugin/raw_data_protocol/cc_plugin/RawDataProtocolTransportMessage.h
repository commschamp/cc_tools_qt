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

#include "RawDataProtocolMessage.h"

#include "raw_data_protocol/Frame.h"

#include "cc_tools_qt/ToolsTransportMessageBase.h"
#include "cc_tools_qt/ToolsTransportProtMessageBase.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

namespace cc_plugin
{

namespace details
{

template <typename TProtMsgBase>
using RawDataProtocolTransportMessageFields =
    std::tuple<
        raw_data_protocol::IdField<typename TProtMsgBase::Field>,
        raw_data_protocol::DataField<typename TProtMsgBase::Field>
    >;

template <typename TProtMsgBase, typename TOpt>
class RawDataProtocolPortTransportMessage : public
    cc_tools_qt::ToolsTransportProtMessageBase<
        TProtMsgBase,
        RawDataProtocolTransportMessageFields<TProtMsgBase>,
        RawDataProtocolPortTransportMessage<TProtMsgBase, TOpt>
    >
{
};

}  // namespace details

class RawDataProtocolTransportMessage : public
    cc_tools_qt::ToolsTransportMessageBase<
        RawDataProtocolMessage,
        details::RawDataProtocolPortTransportMessage,
        RawDataProtocolTransportMessage
    >
{
};

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

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

#include "cc_tools_qt/cc_tools_qt.h"
#include "RawDataProtocolMessage.h"
#include "RawDataProtocolStack.h"

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

using RawDataProtocolTransportMessageFields =
    std::tuple<
        comms::field::IntValue<cc_plugin::RawDataProtocolStack::Message::Field, std::uint8_t, comms::option::def::EmptySerialization>,
        raw_data_protocol::DataField<>
    >;

}  // namespace details

class RawDataProtocolTransportMessage : public
    cc_tools_qt::TransportMessageBase<
        cc_plugin::RawDataProtocolStack::Message,
        details::RawDataProtocolTransportMessageFields>
{
    typedef cc_tools_qt::TransportMessageBase<
        cc_plugin::RawDataProtocolStack::Message,
        details::RawDataProtocolTransportMessageFields> Base;
public:
    enum FieldIdx
    {
        FieldIdx_NoId,
        FieldIdx_Payload,
        FieldIdx_NumOfValues
    };

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override;

};

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

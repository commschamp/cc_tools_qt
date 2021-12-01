//
// Copyright 2016 - 2021 (C). Alex Robenko. All rights reserved.
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
#include "cc_plugin/Message.h"
#include "cc_plugin/Stack.h"

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

using TransportMessageFields =
    std::tuple<
        comms::field::NoValue<cc_plugin::Stack::Message::Field>,
        raw_data_protocol::DataField<>
    >;

}  // namespace details

class TransportMessage : public
    cc_tools_qt::TransportMessageBase<
        cc_plugin::Stack::Message,
        details::TransportMessageFields>
{
    typedef cc_tools_qt::TransportMessageBase<
        cc_plugin::Stack::Message,
        details::TransportMessageFields> Base;
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

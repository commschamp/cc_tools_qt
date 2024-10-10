//
// Copyright 2015 - 2019 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsTransportProtMessageBase.h"
#include "demo/DemoMessage.h"
#include "demo/Stack.h"

#include "comms/options.h"

namespace demo
{

namespace cc_plugin
{

using DemoTransportMessageFields =
    std::tuple<
        demo::SyncField,
        demo::LengthField,
        demo::MsgIdField,
        demo::VersionField,
        demo::DataField<>,
        demo::ChecksumField
    >;

template <typename TMsgBase, typename... TOptions>
class DemoTransportProtMessage : public
    cc_tools_qt::ToolsTransportProtMessageBase<
        TMsgBase,
        DemoTransportMessageFields,
        DemoTransportProtMessage<TMsgBase, TOptions...>
    >
{
    using Base = 
        cc_tools_qt::ToolsTransportProtMessageBase<
            TMsgBase,
            DemoTransportMessageFields,
            DemoTransportProtMessage<TMsgBase, TOptions...>
        >;    
public:
    COMMS_MSG_FIELDS_NAMES(sync, len, id, version, data, checksum);

    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t size)
    {
        static const auto ChecksumLen =
            sizeof(demo::ChecksumField::ValueType);
                    
        size -= ChecksumLen;
        auto es = Base::template doReadUntilAndUpdateLen<FieldIdx_checksum>(iter, size);
        if (es == comms::ErrorStatus::Success) {
            size += ChecksumLen;
            es = Base::template doReadFrom<FieldIdx_checksum>(iter, size);
        }

        return es;
    }            
};

}  // namespace cc_plugin

}  // namespace demo


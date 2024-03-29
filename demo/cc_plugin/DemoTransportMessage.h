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

#include "cc_tools_qt/cc_tools_qt.h"
#include "cc_plugin/DemoMessage.h"
#include "cc_plugin/DemoStack.h"

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


class DemoTransportMessage : public
    cc_tools_qt::TransportMessageBase<
        cc_plugin::DemoMessage,
        DemoTransportMessageFields
    >
{
public:
    enum FieldIdx
    {
        FieldIdx_Sync,
        FieldIdx_Len,
        FieldIdx_Id,
        FieldIdx_Version,
        FieldIdx_Payload,
        FieldIdx_Checksum,
        FieldIdx_NumOfValues
    };

    static_assert(FieldIdx_NumOfValues == std::tuple_size<DemoTransportMessageFields>::value,
            "Wrong indices");

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override;
    virtual comms::ErrorStatus readImpl(ReadIterator& iter, std::size_t size) override;
};

}  // namespace cc_plugin

}  // namespace demo


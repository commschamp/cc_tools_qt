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
#include "raw_data_protocol/Message.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

namespace cc_plugin
{

template <typename... TOptions>
class RawDataProtocolMessageT :
    public cc_tools_qt::MessageBase<raw_data_protocol::MessageT, TOptions...>
{
    typedef cc_tools_qt::MessageBase<raw_data_protocol::MessageT, TOptions...> Base;
public:
    virtual ~RawDataProtocolMessageT() {}

protected:

    virtual QString idAsStringImpl() const override
    {
        return "-";
    }

    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        static const QVariantList Props;
        return Props;
    }
};

typedef RawDataProtocolMessageT<> RawDataProtocolMessage;

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

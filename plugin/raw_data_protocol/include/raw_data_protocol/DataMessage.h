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

#include "Message.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

template <typename TFieldBase>
struct DataMessageFields
{
    class data : public
        comms::field::ArrayList<
            TFieldBase,
            std::uint8_t,
            comms::option::HasName
        >
    {
    public:
        static const char* name()
        {
            return "data";
        }        
    };


    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        data
    >;
};

template <typename TMsgBase, typename TOpt>
class DataMessage : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<0>,
        comms::option::FieldsImpl<typename DataMessageFields<typename TMsgBase::Field>::All>,
        comms::option::MsgType<DataMessage<TMsgBase, TOpt>>,
        comms::option::HasName
    >
{
    using Base = 
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<0>,
            comms::option::FieldsImpl<typename DataMessageFields<typename TMsgBase::Field>::All>,
            comms::option::MsgType<DataMessage<TMsgBase, TOpt>>,
            comms::option::HasName
        >;
public:

    DataMessage() = default;
    DataMessage(const DataMessage&) = default;
    DataMessage(DataMessage&& other) = default;
    ~DataMessage() {}
    DataMessage& operator=(const DataMessage&) = default;
    DataMessage& operator=(DataMessage&&) = default;

    COMMS_MSG_FIELDS_NAMES(data);

    static const char* doName()
    {
        return "Raw Data";
    }    
};

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt



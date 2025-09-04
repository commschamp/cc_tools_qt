//
// Copyright 2016 - 2025 (C). Alex Robenko. All rights reserved.
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

/// @file
/// @brief Contains definition of transport layers protocol stack of raw_data_protocol
///     binary protocol.

#pragma once

#include "raw_data_protocol/AllMessages.h"
#include "raw_data_protocol/Message.h"
#include "raw_data_protocol/DefaultOptions.h"

#include "comms/field/IntValue.h"
#include "comms/field/ArrayList.h"
#include "comms/frame/MsgDataLayer.h"
#include "comms/frame/MsgIdLayer.h"
#include "comms/options.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

template <typename TField, typename... TOptions>
class IdField : public
    comms::field::IntValue<
        TField,
        std::uint8_t,
        TOptions...,
        comms::option::def::EmptySerialization,
        comms::option::def::HasName>
{
public:
    static constexpr bool hasSpecials()
    {
        return false;
    }

    static const char* name()
    {
        return "Id";
    }
};

template <typename TField, typename... TOptions>
class DataField : public
    comms::field::ArrayList<
        TField,
        std::uint8_t,
        TOptions...,
        comms::option::def::HasName>
{
public:
    static const char* name()
    {
        return "Data";
    }
};

template <
    typename TMsgBase,
    typename TAllMessages = raw_data_protocol::AllMessages<TMsgBase, raw_data_protocol::DefaultOptions>
>
using Frame =
        comms::frame::MsgIdLayer<
            IdField<typename TMsgBase::Field>,
            TMsgBase,
            TAllMessages,
            comms::frame::MsgDataLayer<comms::option::def::FieldType<DataField<typename TMsgBase::Field>>>
        >;

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt




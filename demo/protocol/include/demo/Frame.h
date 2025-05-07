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
/// @brief Contains definition of transport layers protocol stack of demo
///     binary protocol.

#pragma once

#include "comms/comms.h"

#include "demo/MsgId.h"
#include "demo/DemoMessage.h"
#include "demo/FieldBase.h"
#include "demo/AllMessages.h"

namespace demo
{

/// @brief Field representing synchronisation information in
///     message wrapping.
/// @details Expects <b>0xab 0xbc</b>sequence.
class SyncField : public
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::def::DefaultNumValue<0xabcd>,
        comms::option::def::ValidNumValueRange<0xabcd, 0xabcd>,
        comms::option::def::HasName
    >
{
public:
    static constexpr bool hasSpecials()
    {
        return false;
    }
    
    static const char* name()
    {
        return "SYNC";
    }
};    

/// @brief Field representing last two checksum bytes in message wrapping.
class ChecksumField : public
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::def::HasName
    >
{
public:
    static constexpr bool hasSpecials()
    {
        return false;
    }
    
    static const char* name()
    {
        return "CHECKSUM";
    }
};  

/// @brief Field representing remaining length in message wrapping.
class LengthField : public
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::def::NumValueSerOffset<sizeof(std::uint16_t)>,
        comms::option::def::DisplayOffset<sizeof(std::uint16_t)>,
        comms::option::def::HasName
    >
{
public:
    static constexpr bool hasSpecials()
    {
        return false;
    }
    
    static const char* name()
    {
        return "LENGTH";
    }
};  

/// @brief Field representing message ID in message wrapping.
class MsgIdField : public
    comms::field::EnumValue<
        FieldBase,
        MsgId,
        comms::option::def::ValidNumValueRange<0, MsgId_NumOfValues - 1>,
        comms::option::def::HasName
    >
{
    using Base = 
        comms::field::EnumValue<
            FieldBase,
            MsgId,
            comms::option::def::ValidNumValueRange<0, MsgId_NumOfValues - 1>,
            comms::option::def::HasName
        >;    
public:
    using ValueType = typename Base::ValueType;
    using ValueNameInfo = const char*;
    using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

    static ValueType firstValue() 
    {
        return MsgId_IntValues;
    }

    static ValueType lastValue() 
    {
        return MsgId_Bundles;
    }    

    static ValueType valuesLimit() 
    {
        return MsgId_NumOfValues;
    }          

    static const char* name()
    {
        return "ID";
    }      

    static const char* valueName(ValueType val)
    {
        auto namesMapInfo = valueNamesMap();
        if (namesMapInfo.second <= static_cast<std::size_t>(val)) {
            return nullptr;
        }

        return namesMapInfo.first[static_cast<std::size_t>(val)];
    }   

    static ValueNamesMapInfo valueNamesMap()
    {
        static const char* Map[] = {
            "IntValues",
            "EnumValues",
            "BitmaskValues",
            "Bitfields",
            "Strings",
            "Lists",
            "Optionals",
            "FloatValues",
            "Variants",
            "Bundles",
        };
        static const std::size_t MapSize = std::extent<decltype(Map)>::value;

        return std::make_pair(&Map[0], MapSize);
    }    
};  

/// @brief Field representing full message payload.
template <typename... TOptions>
class DataField : public 
    comms::field::ArrayList<
        FieldBase,
        std::uint8_t,
        TOptions...,
        comms::option::def::HasName
    >
{
public:
    static const char* name()
    {
        return "PAYLOAD";
    }
};  

/// @brief Assembled protocol stack layers.
/// @details Extended by @ref Stack
template <
    typename TMsgBase,
    typename TMessages,
    typename TMsgAllocOptions = comms::option::EmptyOption>
using StackBase =
    comms::frame::SyncPrefixLayer<
        SyncField,
        comms::frame::ChecksumLayer<
            ChecksumField,
            comms::frame::checksum::BasicSum<std::uint16_t>,
            comms::frame::MsgSizeLayer<
                LengthField,
                comms::frame::MsgIdLayer<
                    MsgIdField,
                    TMsgBase,
                    TMessages,
                    comms::frame::TransportValueLayer<
                        VersionField,
                        DemoMessage<>::TransportFieldIdx_version,
                        comms::frame::MsgDataLayer<
                            comms::option::def::FieldType<DataField<>>
                        >
                    >,
                    TMsgAllocOptions
                >
            >
        >
    >;


/// @brief Definition of Demo binary protocol stack of layers.
/// @details It is used to process incoming binary stream of data and create
///     allocate message objects for received messages. It also responsible to
///     serialise outgoing messages and wrap their payload with appropriate transport
///     information. See <b>Protocol Stack Tutorial</b>
///     page in @b COMMS library tutorial for more information.@n
///     The outermost layer is
///     @b comms::frame::SyncPrefixLayer.
///     Please see its documentation for public interface description.
/// @tparam TMsgBase Interface class for all the messages, expected to be some
///     variant of demo::MessageT class with options.
/// @tparam TMessages Types of all messages that this protocol stack must
///     identify during read and support creation of proper message object.
///     The types of the messages must be bundled in
///     <a href="http://en.cppreference.com/w/cpp/utility/tuple">std::tuple</a>.
/// @tparam TMsgAllocOptions Template parameter(s) passed as options to
///     @b comms::frame::MsgIdLayer
///     protocol layer in @b COMMS library. They are used to specify whether
///     dynamic memory allocation is allowed or "in place" allocation for
///     message objects must be implemented. It is expected to be either
///     single @b COMMS library option or multiple options bundled in
///     <a href="http://en.cppreference.com/w/cpp/utility/tuple">std::tuple</a>.
/// @tparam TDataFieldStorageOptions Extra parameter(s) that are passed
///     to @b comms::frame::MsgDataLayer rotocol layer in @b COMMS library.
///     It may be used to choose storage
///     type of the payload field for "caching" read/write operations.
template <
    typename TMsgBase,
    typename TMessages = demo::AllMessages<TMsgBase, demo::DefaultOptions>,
    typename TMsgAllocOptions = comms::option::EmptyOption>
class Frame : public StackBase<TMsgBase, TMessages, TMsgAllocOptions>
{
    using Base = StackBase<TMsgBase, TMessages, TMsgAllocOptions>;
public:
    COMMS_PROTOCOL_LAYERS_ACCESS(payload, version, id, size, checksum, sync);
};

}  // namespace demo




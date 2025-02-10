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
/// @brief Contains definition of Lists message and its fields.

#pragma once

#include "comms/fields.h"
#include "comms/MessageBase.h"
#include "demo/MsgId.h"
#include "demo/FieldBase.h"
#include "demo/DefaultOptions.h"

#include <iostream>

namespace demo
{

namespace message
{

/// @brief Accumulates details of all the Lists message fields.
/// @tparam TOpt Extra options
/// @see Lists
template <typename TOpt = demo::DefaultOptions>
struct ListsFields
{
    /// @brief Raw data list that uses 2 bytes size prefix
    class field1 : public
        comms::field::ArrayList<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::ListsFields::field1,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    demo::FieldBase,
                    std::uint16_t
                >
            >,
            comms::option::def::HasName
        >
    {
    public:
        static const char* name()
        {
            return "field1";
        }                 
    };

    /// @brief Raw data list that uses 1 bytes serialisation length prefix
    class field2 : public
        comms::field::ArrayList<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::ListsFields::field2,
            comms::option::def::HasName,
            comms::option::SequenceSerLengthFieldPrefix<
                comms::field::IntValue<
                    demo::FieldBase,
                    std::uint8_t
                >
            >
        >
    {
    public:
        static const char* name()
        {
            return "field2";
        }                 
    };       

    /// @brief Element of @ref field3 list
    class field3Element : public
        comms::field::IntValue<
            demo::FieldBase,
            std::int16_t,
            typename TOpt::message::ListsFields::field3Element,
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
            return "element";
        }                 
    };        

    /// @brief List of 2 bytes integer value fields, with fixed size of 3 elements
    class field3 : public
        comms::field::ArrayList<
            demo::FieldBase,
            field3Element,
            typename TOpt::message::ListsFields::field3,
            comms::option::SequenceFixedSize<3>,
            comms::option::def::HasName
        >
    {
    public:
        static const char* name()
        {
            return "field3";
        }                 
    }; 

    /// @brief Element of @ref field4 list
    class field4Element : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint16_t,
            typename TOpt::message::ListsFields::field4Element,
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
            return "element";
        }                 
    }; 

    /// @brief List of 2 bytes integer value fields, prefixed with
    ///     2 bytes serialisation length
    class field4 : public
        comms::field::ArrayList<
            demo::FieldBase,
            field4Element,
            typename TOpt::message::ListsFields::field4,
            comms::option::def::HasName,
            comms::option::SequenceSerLengthFieldPrefix<
                comms::field::IntValue<
                    demo::FieldBase,
                    std::uint16_t
                >
            >
        >
    {
    public:
        static const char* name()
        {
            return "field4";
        }                 
    }; 

    /// Scope for members of @ref field5Element
    struct field5Members
    {
        /// @brief 2 byte unsigned integer field
        class mem1 : public
            comms::field::IntValue<
                demo::FieldBase,
                std::uint16_t,
                typename TOpt::message::ListsFields::field5Members::mem1,
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
                return "member1";
            }                 
        }; 

        /// @brief 1 byte unsigned integer field
        class mem2 : public
            comms::field::IntValue<
                demo::FieldBase,
                std::int8_t,
                typename TOpt::message::ListsFields::field5Members::mem2,
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
                return "member2";
            }                 
        }; 

        /// @brief string with serialisation length prefix. The prefix is of variable
        ///         length, i.e. uses base-128 encoding.
        class mem3 : public
            comms::field::String<
                demo::FieldBase,
                typename TOpt::message::ListsFields::field5Members::mem3,
                comms::option::def::HasName,
                comms::option::SequenceSizeFieldPrefix<
                    comms::field::IntValue<
                        demo::FieldBase,
                        std::uint32_t,
                        comms::option::VarLength<1, 4>
                    >
                >
            >
        {
        public:
            static const char* name()
            {
                return "member3";
            }                 
        }; 
    };

    /// @brief Element of @ref field5 list
    class field5Element : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                typename field5Members::mem1,
                typename field5Members::mem2,
                typename field5Members::mem3
            >,
            typename TOpt::message::ListsFields::field5Element,
            comms::option::def::HasName
        >
    {
    public:
        static const char* name()
        {
            return "element";
        }                 
    }; 

    /// @brief List of bundles
    /// @details Every bundle contains:
    ///     @li 2 byte unsigned integer field
    ///     @li 1 byte unsigned integer field
    ///     @li string with serialisation length prefix. The prefix is of variable
    ///         length, i.e. uses base-128 encoding.
    ///
    ///     Every list element (bundle) is prefixed with its total serialisation
    ///     length. The length info is written using base-128 encoding.
    ///
    ///     The list itself is prefixed with total serialisation length. The length
    ///     info is written using base-128 encoding.
    class field5 : public
        comms::field::ArrayList<
            demo::FieldBase,
            field5Element,
            typename TOpt::message::ListsFields::field5,
            comms::option::def::HasName,
            comms::option::SequenceSerLengthFieldPrefix<
                comms::field::IntValue<
                    demo::FieldBase,
                    std::uint32_t,
                    comms::option::VarLength<1, 4>
                >
            >,
            comms::option::SequenceElemSerLengthFieldPrefix<
                comms::field::IntValue<
                    demo::FieldBase,
                    std::uint8_t,
                    comms::option::VarLength<1, 4>
                >
            >
        >
    {
    public:
        static const char* name()
        {
            return "field5";
        }                 
    }; 

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1,
        field2,
        field3,
        field4,
        field5
    >;
};

/// @brief Accumulates various list fields.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref ListsFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class Lists : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Lists,
        comms::option::StaticNumIdImpl<MsgId_Lists>,
        comms::option::FieldsImpl<typename ListsFields<TOpt>::All>,
        comms::option::MsgType<Lists<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Lists,
            comms::option::StaticNumIdImpl<MsgId_Lists>,
            comms::option::FieldsImpl<typename ListsFields<TOpt>::All>,
            comms::option::MsgType<Lists<TMsgBase, TOpt> >,
            comms::option::HasName
        >;

    static const bool AreFieldsVersionDependent = Base::areFieldsVersionDependent();
    static_assert(!AreFieldsVersionDependent, "Fields mustn't be version dependent");
public:

    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    COMMS_MSG_FIELDS_NAMES(field1, field2, field3, field4, field5);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when call to doMinLength() and
    // is performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static_assert(MsgMinLen == 12, "Unexpected min serialisation length");

    /// @brief Default constructor
    Lists() = default;

    /// @brief Copy constructor
    Lists(const Lists&) = default;

    /// @brief Move constructor
    Lists(Lists&& other) = default;

    /// @brief Destructor
    ~Lists() noexcept = default;

    /// @brief Copy assignment
    Lists& operator=(const Lists&) = default;

    /// @brief Move assignment
    Lists& operator=(Lists&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "Lists";
    }
};

}  // namespace message

}  // namespace demo



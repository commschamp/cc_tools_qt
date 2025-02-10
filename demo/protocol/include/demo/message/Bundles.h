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
/// @brief Contains definition of Bundles message and its fields.

#pragma once

#include "comms/fields.h"
#include "comms/MessageBase.h"
#include "demo/MsgId.h"
#include "demo/FieldBase.h"
#include "demo/DefaultOptions.h"

namespace demo
{


namespace message
{

/// @brief Accumulates details of all the Bundles message fields.
/// @tparam TOpt Extra options
/// @see Bundles
template <typename TOpt = demo::DefaultOptions>
struct BundlesFields
{
    /// @brief First member that can be stored in @ref field1 bundle field.
    class field1_mem1 : public
            comms::field::IntValue<
                demo::FieldBase,
                std::uint8_t,
                typename TOpt::message::BundlesFields::field1_mem1,
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
            return "field1_mem1";
        }                 
    };

    /// @brief Second member that can be stored in @ref field1 bundle field.
    class field1_mem2 : public
        comms::field::String<
            demo::FieldBase,
            typename TOpt::message::BundlesFields::field1_mem2,
            comms::option::def::SequenceFixedSize<8>,
            comms::option::def::HasName
        >
    {
    public:
        static const char* name()
        {
            return "field1_mem2";
        }                 
    };

    /// @brief Bundle field containing length and fixed size string.
    class field1 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                field1_mem1,
                field1_mem2
            >,
            comms::option::def::RemLengthMemberField<0>,
            typename TOpt::message::BundlesFields::field1,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    field1_mem1,
                    field1_mem2
                >,
                comms::option::def::RemLengthMemberField<0>,
                typename TOpt::message::BundlesFields::field1,
                comms::option::def::HasName
            >;            

    public:
        /// @brief Allow access to internal fields.
        COMMS_FIELD_MEMBERS_NAMES(length, str)

        static const char* name()
        {
            return "field1";
        }        
    };

    /// @brief First member that can be stored in @ref field2 bundle field.
    class field2_mem1 : public
            comms::field::IntValue<
                demo::FieldBase,
                std::uint8_t,
                typename TOpt::message::BundlesFields::field2_mem1,
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
            return "field2_mem1";
        }                 
    };

    /// @brief Second member that can be stored in @ref field2 bundle field.
    class field2_mem2 : public
        comms::field::ArrayList<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::BundlesFields::field2_mem2,
            comms::option::def::SequenceFixedSize<8>,
            comms::option::def::HasName
        >
    {
    public:
        static const char* name()
        {
            return "field2_mem2";
        }                 
    };

    /// @brief Bundle field containing length and fixed size data.
    class field2 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                field2_mem1,
                field2_mem2
            >,
            comms::option::def::RemLengthMemberField<0>,
            typename TOpt::message::BundlesFields::field2,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    field2_mem1,
                    field2_mem2
                >,
                comms::option::def::RemLengthMemberField<0>,
                typename TOpt::message::BundlesFields::field2,
                comms::option::def::HasName
            >;            

    public:
        /// @brief Allow access to internal fields.
        COMMS_FIELD_MEMBERS_NAMES(length, data)

        static const char* name()
        {
            return "field2";
        }         
    };
        
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1,
        field2
    >;
};

/// @brief Accumulates various string fields.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref BundlesFields for definition of the fields this message contains.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class Bundles : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Bundles,
        comms::option::StaticNumIdImpl<MsgId_Bundles>,
        comms::option::FieldsImpl<typename BundlesFields<TOpt>::All>,
        comms::option::MsgType<Bundles<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Bundles,
            comms::option::StaticNumIdImpl<MsgId_Bundles>,
            comms::option::FieldsImpl<typename BundlesFields<TOpt>::All>,
            comms::option::MsgType<Bundles<TMsgBase, TOpt> >,
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
    COMMS_MSG_FIELDS_NAMES(field1, field2);

    /// @brief Default constructor
    Bundles() = default;

    /// @brief Copy constructor
    Bundles(const Bundles&) = default;

    /// @brief Move constructor
    Bundles(Bundles&& other) = default;

    /// @brief Destructor
    ~Bundles() noexcept = default;

    /// @brief Copy assignment
    Bundles& operator=(const Bundles&) = default;

    /// @brief Move assignment
    Bundles& operator=(Bundles&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "Bundles";
    }
};

}  // namespace message

}  // namespace demo


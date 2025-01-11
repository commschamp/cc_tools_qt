//
// Copyright 2016 - 2018 (C). Alex Robenko. All rights reserved.
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
/// @brief Contains definition of Bitfields message and its fields.

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

/// @brief Accumulates details of all the Bitfields message fields.
/// @tparam TOpt Extra options
/// @see Bitfields
template <typename TOpt = demo::DefaultOptions>
struct BitfieldsFields
{
    /// @brief Simple 4 bits bitmask value, residing as a member in @ref field1 bitfield.
    /// @details It uses 4 bits of @ref field1. The "bit 3" of the bitmask is
    ///     defined to be reserved and must be 0.
    struct field1_bitmask : public
        comms::field::BitmaskValue<
            demo::FieldBase,
            typename TOpt::message::BitfieldsFields::field1_bitmask,
            comms::option::def::FixedLength<1>,
            comms::option::def::FixedBitLength<4>,
            comms::option::def::BitmaskReservedBits<0xf8, 0>,
            comms::option::def::HasName
        >
    {
        /// @brief Provide names for internal bits.
        /// @details See definition of @b COMMS_BITMASK_BITS_SEQ_NOTEMPLATE macro
        ///     related to @b comms::field::BitmaskValue class from COMMS library
        ///     for details.
        COMMS_BITMASK_BITS_SEQ_NOTEMPLATE(bit0, bit1, bit2);

        static const char* name()
        {
            return "field1_bitmask";
        }

        static const char* bitName(std::size_t idx)
        {
            static const char* Map[] = {
                "bit0",
                "bit1",
                "bit2",
            };

            static const std::size_t MapSize = std::extent<decltype(Map)>::value;
            static_assert(MapSize == BitIdx_numOfValues, "Invalid map");

            if (MapSize <= idx) {
                return nullptr;
            }

            return Map[idx];
        }           
    };

    /// @brief Enumeration type for the @ref field1_enum
    enum class Field1Enum : std::uint8_t
    {
        Value1, ///< Value1
        Value2, ///< Value2
        Value3, ///< Value3
        NumOfValues ///< Number of available values
    };

    /// @brief Enumeration field, that consumes 2 bits in @ref field1 bitfield.
    class field1_enum : public
        comms::field::EnumValue<
            demo::FieldBase,
            Field1Enum,
            typename TOpt::message::BitfieldsFields::field1_enum,
            comms::option::def::ValidNumValueRange<static_cast<int>(0), static_cast<int>(Field1Enum::NumOfValues) - 1>,
            comms::option::def::FixedBitLength<2>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::EnumValue<
                demo::FieldBase,
                Field1Enum,
                typename TOpt::message::BitfieldsFields::field1_enum,
                comms::option::def::ValidNumValueRange<static_cast<int>(0), static_cast<int>(Field1Enum::NumOfValues) - 1>,
                comms::option::def::FixedBitLength<2>,
                comms::option::def::HasName
            >;        
    public:
        using ValueType = typename Base::ValueType;
        using ValueNameInfo = const char*;
        using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

        static ValueType firstValue() 
        {
            return ValueType::Value1;
        }

        static ValueType lastValue() 
        {
            return ValueType::Value3;
        }        

        static ValueType valuesLimit() 
        {
            return ValueType::NumOfValues;
        }  

        static const char* name()
        {
            return "field1_enum";
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
                "Value1",
                "Value2",
                "Value3",
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }           
    };


    /// @brief Integer field, that consumes 6 bits in @ref field1 bitfield.
    class field1_int1 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::BitfieldsFields::field1_int1,
            comms::option::def::FixedBitLength<6>,
            comms::option::def::ValidNumValueRange<0, 0x3f>,
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
            return "field1_int1";
        }        
    };

    /// @brief Integer field, that consumes 4 bits in @ref field1 bitfield.
    class field1_int2 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::BitfieldsFields::field1_int2,
            comms::option::def::FixedBitLength<4>,
            comms::option::def::ValidNumValueRange<0, 0xf>,
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
            return "field1_int2";
        }        
    };

    /// @brief Bitfield containing @ref field1_bitmask, @ref field1_enum,
    ///     @ref field1_int1, and @ref field1_int2 as its internal members
    class field1 : public
        comms::field::Bitfield<
            demo::FieldBase,
            std::tuple<
                field1_bitmask,
                field1_enum,
                field1_int1,
                field1_int2
            >,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bitfield<
                demo::FieldBase,
                std::tuple<
                    field1_bitmask,
                    field1_enum,
                    field1_int1,
                    field1_int2
                >,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bitfield class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b member1 for @ref field1_bitmask
        ///     @b member2 for @ref field1_enum
        ///     @b member3 for @ref field1_int1
        ///     @b member4 for @ref field1_int2
        ///
        COMMS_FIELD_MEMBERS_NAMES(member1, member2, member3, member4);

        static const char* name()
        {
            return "field1";
        }          
    };

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1
    >;
};

/// @brief Accumulates various bitfield fields.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref BitfieldsFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class Bitfields : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Bitfields,
        comms::option::StaticNumIdImpl<MsgId_Bitfields>,
        comms::option::FieldsImpl<typename BitfieldsFields<TOpt>::All>,
        comms::option::MsgType<Bitfields<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Bitfields,
            comms::option::StaticNumIdImpl<MsgId_Bitfields>,
            comms::option::FieldsImpl<typename BitfieldsFields<TOpt>::All>,
            comms::option::MsgType<Bitfields<TMsgBase, TOpt> >,
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
    COMMS_MSG_FIELDS_NAMES(field1);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when calls to doMinLength() and
    // doMaxLength() are performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 2, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 2, "Unexpected max serialisation length");

    /// @brief Default constructor
    Bitfields() = default;

    /// @brief Copy constructor
    Bitfields(const Bitfields&) = default;

    /// @brief Move constructor
    Bitfields(Bitfields&&) = default;

    /// @brief Destructor
    ~Bitfields() noexcept = default;

    /// @brief Copy assignment
    Bitfields& operator=(const Bitfields&) = default;

    /// @brief Move assignment
    Bitfields& operator=(Bitfields&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "Bitfields";
    }
};

}  // namespace message

}  // namespace demo



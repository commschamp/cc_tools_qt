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
/// @brief Contains definition of FloatValues message and its fields.

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

/// @brief Accumulates details of all the FloatValues message fields.
/// @tparam TOpt Extra options
/// @see FloatValues
template <typename TOpt = demo::DefaultOptions>
struct FloatValuesFields
{
    /// @brief Simple 4 byte IEEE 754 floating point value.
    class field1 : public
        comms::field::FloatValue<
            demo::FieldBase,
            float,
            typename TOpt::message::FloatValuesFields::field1,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::FloatValue<
                demo::FieldBase,
                float,
                typename TOpt::message::FloatValuesFields::field1,
                comms::option::def::HasName
            >;        
    public:
        using ValueType = typename Base::ValueType;
        using SpecialNameInfo = std::pair<ValueType, const char*>;
        using SpecialNamesMapInfo = std::pair<const SpecialNameInfo*, std::size_t>;

        static constexpr bool hasSpecials()
        {
            return true;
        }        

        static const char* name()
        {
            return "field1";
        }   

        static SpecialNamesMapInfo specialNamesMap()
        {
            static const SpecialNameInfo Map[] = {
                std::make_pair(ValueType(0.1), "S1"),
                std::make_pair(std::numeric_limits<ValueType>::quiet_NaN(), "S2"),
                std::make_pair(std::numeric_limits<ValueType>::infinity(), "S3"),
                std::make_pair(-std::numeric_limits<ValueType>::infinity(), "S4"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        } 

        static constexpr unsigned displayDecimals()
        {
            return 3U;
        }                                 
    };

    /// @brief Simple 8 byte IEEE 754 floating point value.
    class field2 : public
        comms::field::FloatValue<
            demo::FieldBase,
            double,
            typename TOpt::message::FloatValuesFields::field2,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::FloatValue<
                demo::FieldBase,
                double,
                typename TOpt::message::FloatValuesFields::field2,
                comms::option::def::HasName
            >;
    public:
        using ValueType = typename Base::ValueType;
        using SpecialNameInfo = std::pair<ValueType, const char*>;
        using SpecialNamesMapInfo = std::pair<const SpecialNameInfo*, std::size_t>;

        static constexpr bool hasSpecials()
        {
            return true;
        }          

        static const char* name()
        {
            return "field2";
        }    

        static SpecialNamesMapInfo specialNamesMap()
        {
            static const SpecialNameInfo Map[] = {
                std::make_pair(ValueType(0.5), "S1"),
                std::make_pair(std::numeric_limits<ValueType>::quiet_NaN(), "S2"),
                std::make_pair(std::numeric_limits<ValueType>::infinity(), "S3"),
                std::make_pair(-std::numeric_limits<ValueType>::infinity(), "S4"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }      

        static constexpr unsigned displayDecimals()
        {
            return 0U;
        }                          
    };

    /// @brief Floating point value serialised as integer with (1e-2) scaling ratio.
    class field3 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::FloatValuesFields::field3,
            comms::option::ScalingRatio<1, 100>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::IntValue<
                demo::FieldBase,
                std::uint8_t,
                typename TOpt::message::FloatValuesFields::field3,
                comms::option::ScalingRatio<1, 100>,
                comms::option::def::HasName
            >;
    public:
        using ValueType = typename Base::ValueType;
        using SpecialNameInfo = std::pair<ValueType, const char*>;
        using SpecialNamesMapInfo = std::pair<const SpecialNameInfo*, std::size_t>;

        static constexpr bool hasSpecials()
        {
            return true;
        }

        static const char* name()
        {
            return "field3";
        }  

        static SpecialNamesMapInfo specialNamesMap()
        {
            static const SpecialNameInfo Map[] = {
                std::make_pair(ValueType(123), "S1"),
                std::make_pair(ValueType(22), "S2"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }    

        static constexpr unsigned displayDecimals()
        {
            return 2U;
        }                             
    };

    /// @brief Floating point value serialised as 5 byte integer with (1e-11) scaling ratio.
    class field4 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::int64_t,
            typename TOpt::message::FloatValuesFields::field4,
            comms::option::ScalingRatio<1, 100000000000>,
            comms::option::FixedLength<5>,
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
            return "field4";
        } 

        static constexpr unsigned displayDecimals()
        {
            return 11U;
        }                          
    };

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1,
        field2,
        field3,
        field4
    >;

};

/// @brief Accumulates various floating point values.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref FloatValuesFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class FloatValues : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::FloatValues,
        comms::option::StaticNumIdImpl<MsgId_FloatValues>,
        comms::option::FieldsImpl<typename FloatValuesFields<TOpt>::All>,
        comms::option::MsgType<FloatValues<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::FloatValues,
            comms::option::StaticNumIdImpl<MsgId_FloatValues>,
            comms::option::FieldsImpl<typename FloatValuesFields<TOpt>::All>,
            comms::option::MsgType<FloatValues<TMsgBase, TOpt> >,
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
    COMMS_MSG_FIELDS_NAMES(field1, field2, field3, field4);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when calls to doMinLength() and
    // doMaxLength() are performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 18, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 18, "Unexpected max serialisation length");

    /// @brief Default constructor
    FloatValues() = default;

    /// @brief Copy constructor
    FloatValues(const FloatValues&) = default;

    /// @brief Move constructor
    FloatValues(FloatValues&& other) = default;

    /// @brief Destructor
    ~FloatValues() noexcept = default;

    /// @brief Copy assignment
    FloatValues& operator=(const FloatValues&) = default;

    /// @brief Move assignment
    FloatValues& operator=(FloatValues&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "FloatValues";
    }
};

}  // namespace message

}  // namespace demo




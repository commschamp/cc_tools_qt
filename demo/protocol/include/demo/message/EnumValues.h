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
/// @brief Contains definition of EnumValues message and its fields.

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

/// @brief Accumulates details of all the EnumValues message fields.
/// @tparam TOpt Extra options
/// @see EnumValues
template <typename TOpt = demo::DefaultOptions>
struct EnumValuesFields
{
    /// @brief Enumeration type for the @ref field1
    /// @details The values are sequential and serialised as single byte
    enum class ValuesField1 : std::uint8_t
    {
        Value1, ///< Value1
        Value2, ///< Value2
        Value3, ///< Value3
        NumOfValues ///< Number of available values
    };

    /// @brief Simple 1 byte enumeration value.
    class field1 : public
        comms::field::EnumValue<
            demo::FieldBase,
            ValuesField1,
            typename TOpt::message::EnumValuesFields::field1,
            comms::option::ValidNumValueRange<static_cast<int>(0), static_cast<int>(ValuesField1::NumOfValues) - 1>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::EnumValue<
                demo::FieldBase,
                ValuesField1,
                typename TOpt::message::EnumValuesFields::field1,
                comms::option::ValidNumValueRange<static_cast<int>(0), static_cast<int>(ValuesField1::NumOfValues) - 1>,
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
            return "field1";
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
                "Value3"
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }                         
    };

    /// @brief Enumeration type for the @ref field2
    /// @details The values are sparse and signed. They are serialised
    ///     as two bytes.
    enum class ValuesField2 : std::int16_t
    {
        Value1 = -5, ///< Value1
        Value2 = 100, ///< Value2
        Value3 = 130, ///< Value3,
        Value4 = 1028 ///< Value4
    };

    /// @brief Enumeration, that has sparse and signed values, as well as
    ///     serialised using 2 bytes.
    class field2 : public
        comms::field::EnumValue<
            demo::FieldBase,
            ValuesField2,
            typename TOpt::message::EnumValuesFields::field2,
            comms::option::DefaultNumValue<static_cast<int>(ValuesField2::Value1)>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::EnumValue<
                demo::FieldBase,
                ValuesField2,
                typename TOpt::message::EnumValuesFields::field2,
                comms::option::DefaultNumValue<static_cast<int>(ValuesField2::Value1)>,
                comms::option::def::HasName
            >;

    public:
        using ValueType = typename Base::ValueType;
        using ValueNameInfo = std::pair<ValueType, const char*>;
        using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

        static ValueType firstValue() 
        {
            return ValueType::Value1;
        }

        static ValueType lastValue() 
        {
            return ValueType::Value4;
        }        

        static ValueType valuesLimit() 
        {
            return static_cast<ValueType>(static_cast<int>(lastValue()) + 1);
        }         

        static const char* name()
        {
            return "field2";
        }   

        // @brief Custom validity check
        bool valid() const
        {
            return
                (Base::value() == ValueType::Value1) ||
                (Base::value() == ValueType::Value2) ||
                (Base::value() == ValueType::Value3) ||
                (Base::value() == ValueType::Value4);            
        }

        static const char* valueName(ValueType val)
        {
            auto namesMapInfo = valueNamesMap();
            auto begIter = namesMapInfo.first;
            auto endIter = begIter + namesMapInfo.second;
            auto iter = std::lower_bound(
                begIter, endIter, val,
                [](const ValueNameInfo& info, ValueType v) -> bool
                {
                    return info.first < v;
                });

            if ((iter == endIter) || (iter->first != val)) {
                return nullptr;
            }

            return iter->second;
        }   

        static ValueNamesMapInfo valueNamesMap()
        {
            static const ValueNameInfo Map[] = {
                std::make_pair(ValueType::Value1, "Value1"),
                std::make_pair(ValueType::Value2, "Value2"),
                std::make_pair(ValueType::Value3, "Value3"),
                std::make_pair(ValueType::Value4, "Value4")
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }             
    };

    /// @brief Enumeration type for the @ref field3
    /// @details The values are sparse and unsigned. They are serialised
    ///     using base-128 encoding.
    enum class ValuesField3 : std::uint16_t
    {
        Value1 = 5, ///< Value1
        Value2 = 100, ///< Value2
        Value3 = 127, ///< Value3,
        Value4 = 128, ///< Value4
        Value5 = 200 ///< Value5
    };

    /// @brief Enumeration, that has sparse and unsigned values, as well as
    ///     serialised using base-128 encoding.
    class field3 : public
        comms::field::EnumValue<
            demo::FieldBase,
            ValuesField3,
            typename TOpt::message::EnumValuesFields::field3,
            comms::option::VarLength<1, 2>,
            comms::option::DefaultNumValue<static_cast<int>(ValuesField3::Value1)>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::EnumValue<
                demo::FieldBase,
                ValuesField3,
                typename TOpt::message::EnumValuesFields::field3,
                comms::option::VarLength<1, 2>,
                comms::option::DefaultNumValue<static_cast<int>(ValuesField3::Value1)>,
                comms::option::def::HasName
            >;

    public:
        using ValueType = typename Base::ValueType;
        using ValueNameInfo = std::pair<ValueType, const char*>;
        using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

        static ValueType firstValue() 
        {
            return ValueType::Value1;
        }

        static ValueType lastValue() 
        {
            return ValueType::Value5;
        }        

        static ValueType valuesLimit() 
        {
            return static_cast<ValueType>(static_cast<int>(lastValue()) + 1);
        }           

        static const char* name()
        {
            return "field3";
        }   

        bool valid() const
        {
            return
                (Base::value() == ValueType::Value1) ||
                (Base::value() == ValueType::Value2) ||
                (Base::value() == ValueType::Value3) ||
                (Base::value() == ValueType::Value4) ||
                (Base::value() == ValueType::Value5);            
        }

        static const char* valueName(ValueType val)
        {
            auto namesMapInfo = valueNamesMap();
            auto begIter = namesMapInfo.first;
            auto endIter = begIter + namesMapInfo.second;
            auto iter = std::lower_bound(
                begIter, endIter, val,
                [](const ValueNameInfo& info, ValueType v) -> bool
                {
                    return info.first < v;
                });

            if ((iter == endIter) || (iter->first != val)) {
                return nullptr;
            }

            return iter->second;
        }   

        static ValueNamesMapInfo valueNamesMap()
        {
            static const ValueNameInfo Map[] = {
                std::make_pair(ValueType::Value1, "Value1"),
                std::make_pair(ValueType::Value2, "Value2"),
                std::make_pair(ValueType::Value3, "Value3"),
                std::make_pair(ValueType::Value4, "Value4"),
                std::make_pair(ValueType::Value5, "Value5"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }           
    };

    /// @brief Enumeration type for the @ref field4
    /// @details The values are sequential and serialised as single byte
    enum class ValuesField4 : std::uint64_t
    {
        Value1 = 0U, ///< Value1
        Value2 = 0x7fffffffffffffff, ///< Value2
        Value3 = 0xffffffffffffffff, ///< Value3
    };

    /// @brief 8 byte enumeration value.
    class field4 : public
        comms::field::EnumValue<
            demo::FieldBase,
            ValuesField4,
            typename TOpt::message::EnumValuesFields::field4,
            comms::option::ValidBigUnsignedNumValue<static_cast<std::uintmax_t>(ValuesField4::Value1)>,
            comms::option::ValidBigUnsignedNumValue<static_cast<std::uintmax_t>(ValuesField4::Value2)>,
            comms::option::ValidBigUnsignedNumValue<static_cast<std::uintmax_t>(ValuesField4::Value3)>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::EnumValue<
                demo::FieldBase,
                ValuesField4,
                typename TOpt::message::EnumValuesFields::field4,
                comms::option::ValidBigUnsignedNumValue<static_cast<std::uintmax_t>(ValuesField4::Value1)>,
                comms::option::ValidBigUnsignedNumValue<static_cast<std::uintmax_t>(ValuesField4::Value2)>,
                comms::option::ValidBigUnsignedNumValue<static_cast<std::uintmax_t>(ValuesField4::Value3)>,
                comms::option::def::HasName
            >; 
    public:
        using ValueType = typename Base::ValueType;
        using ValueNameInfo = std::pair<ValueType, const char*>;
        using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

        static ValueType firstValue() 
        {
            return ValueType::Value1;
        }

        static ValueType lastValue() 
        {
            return ValueType::Value3;
        }        

        static const char* name()
        {
            return "field4";
        }   

        static const char* valueName(ValueType val)
        {
            auto namesMapInfo = valueNamesMap();
            auto begIter = namesMapInfo.first;
            auto endIter = begIter + namesMapInfo.second;
            auto iter = std::lower_bound(
                begIter, endIter, val,
                [](const ValueNameInfo& info, ValueType v) -> bool
                {
                    return info.first < v;
                });

            if ((iter == endIter) || (iter->first != val)) {
                return nullptr;
            }

            return iter->second;
        }   

        static ValueNamesMapInfo valueNamesMap()
        {
            static const ValueNameInfo Map[] = {
                std::make_pair(ValueType::Value1, "Value1"),
                std::make_pair(ValueType::Value2, "Value2"),
                std::make_pair(ValueType::Value3, "Value3"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
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

/// @brief Accumulates various enum values.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref EnumValuesFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class EnumValues : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::EnumValues,
        comms::option::StaticNumIdImpl<MsgId_EnumValues>,
        comms::option::FieldsImpl<typename EnumValuesFields<TOpt>::All>,
        comms::option::MsgType<EnumValues<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::EnumValues,
            comms::option::StaticNumIdImpl<MsgId_EnumValues>,
            comms::option::FieldsImpl<typename EnumValuesFields<TOpt>::All>,
            comms::option::MsgType<EnumValues<TMsgBase, TOpt> >,
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
    static_assert(MsgMinLen == 12, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 13, "Unexpected max serialisation length");

    /// @brief Default constructor
    EnumValues() = default;

    /// @brief Copy constructor
    EnumValues(const EnumValues&) = default;

    /// @brief Move constructor
    EnumValues(EnumValues&& other) = default;

    /// @brief Destructor
    ~EnumValues() noexcept = default;

    /// @brief Copy assignment
    EnumValues& operator=(const EnumValues&) = default;

    /// @brief Move assignment
    EnumValues& operator=(EnumValues&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "EnumValues";
    }

};

}  // namespace message

}  // namespace demo




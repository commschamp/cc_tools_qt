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
/// @brief Contains definition of Optionals message and its fields.

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

/// @brief Accumulates details of all the Optionals message fields.
/// @tparam TOpt Extra options
/// @see Optionals
template <typename TOpt = demo::DefaultOptions>
struct OptionalsFields
{
    /// @brief Bitmask that is used to enable/disable other fields
    class field1 : public
        comms::field::BitmaskValue<
            demo::FieldBase,
            typename TOpt::message::OptionalsFields::field1,
            comms::option::FixedLength<1>,
            comms::option::BitmaskReservedBits<0xfc, 0>,
            comms::option::def::HasName
        >
    {
    public:        
        /// @brief Provide names for internal bits.
        /// @details See definition of @b COMMS_BITMASK_BITS macro
        ///     related to @b comms::field::BitmaskValue class from COMMS library
        ///     for details.
        COMMS_BITMASK_BITS(enableField2, enableField3);

        static const char* name()
        {
            return "field1";
        }   

        static const char* bitName(std::size_t idx)
        {
            static const char* Map[] = {
                "enable_field2",
                "enable_field3",
            };

            static const std::size_t MapSize = std::extent<decltype(Map)>::value;
            static_assert(MapSize == BitIdx_numOfValues, "Invalid map");

            if (MapSize <= idx) {
                return nullptr;
            }

            return Map[idx];
        }                
    };

    struct field2Members
    {
        class field2Field : public
            comms::field::IntValue<
                demo::FieldBase,
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
                return "field2";
            }  
        };
    };

    /// @brief Optional 2 bytes unsigned integer value
    /// @details Existence of this field is controlled by bit 0 in @ref field1
    class field2 : public
        comms::field::Optional<
            typename field2Members::field2Field,
            typename TOpt::message::OptionalsFields::field2,
            comms::option::OptionalMissingByDefault,
            comms::option::def::HasName
        >
    {
    public:
        static const char* name()
        {
            return "field2";
        }                 
    }; 

    struct field3Members
    {
        class field3Field : public
            comms::field::String<
                demo::FieldBase,
                comms::option::SequenceSizeFieldPrefix<
                    comms::field::IntValue<
                        demo::FieldBase,
                        std::uint8_t
                    >
                >,
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
                return "field3";
            }  
        };
    };    

    /// @brief Optional string with 1 byte size information prefix.
    /// @details Existence of this field is controlled by bit 1 in @ref field1
    class field3 : public
        comms::field::Optional<
            typename field3Members::field3Field,
            typename TOpt::message::OptionalsFields::field3,
            comms::option::OptionalMissingByDefault,
            comms::option::def::HasName
        >
    {
    public:            
        static const char* name()
        {
            return "field3";
        }  
    };

    struct field4Members
    {
        class field4Field : public
            comms::field::IntValue<
                demo::FieldBase,
                std::int16_t,
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
        };
    };          

    /// @brief Optional 2 bytes signed integer value
    /// @details Existence of this field is controlled by the version
    ///     information in transport framing. If version is 0, this field
    ///     does NOT exist. If the version is 1 or greater, this field does
    ///     exist.
    class field4 : public
        comms::field::Optional<
            typename field4Members::field4Field,
            comms::option::ExistsByDefault,
            comms::option::ExistsSinceVersion<1>,
            comms::option::def::HasName
        >
    {
    public:            
        static const char* name()
        {
            return "field4";
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

/// @brief Accumulates various string fields.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref OptionalsFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class Optionals : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Optionals,
        comms::option::StaticNumIdImpl<MsgId_Optionals>,
        comms::option::FieldsImpl<typename OptionalsFields<TOpt>::All>,
        comms::option::MsgType<Optionals<TMsgBase, TOpt> >,
        comms::option::HasCustomRefresh,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Optionals,
            comms::option::StaticNumIdImpl<MsgId_Optionals>,
            comms::option::FieldsImpl<typename OptionalsFields<TOpt>::All>,
            comms::option::MsgType<Optionals<TMsgBase, TOpt> >,
            comms::option::HasCustomRefresh,
            comms::option::HasName
        >;

    static const bool AreFieldsVersionDependent = Base::areFieldsVersionDependent();
    static_assert(AreFieldsVersionDependent, "Fields must be version dependent");
public:
    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    COMMS_MSG_FIELDS_NAMES(field1, field2, field3, field4);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when call to doMinLength()
    // is performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static_assert(MsgMinLen == 1, "Unexpected min serialisation length");

    /// @brief Default constructor
    Optionals() = default;

    /// @brief Copy constructor
    Optionals(const Optionals&) = default;

    /// @brief Move constructor
    Optionals(Optionals&& other) = default;

    /// @brief Destructor
    ~Optionals() noexcept = default;

    /// @brief Copy assignment
    Optionals& operator=(const Optionals&) = default;

    /// @brief Move assignment
    Optionals& operator=(Optionals&&) = default;

    /// @brief Implement custom read functionality
    /// @param[in, out] iter Reference to iterator used for reading.
    /// @param[in] len Number of remaining bytes in the buffer to read
    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t len)
    {
        Base::doFieldsVersionUpdate();

        auto es = Base::template doReadUntilAndUpdateLen<FieldIdx_field2>(iter, len);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        using Field1Type = typename std::decay<decltype(field_field1())>::type;
        auto field2Mode = comms::field::OptionalMode::Missing;
        if (field_field1().getBitValue(Field1Type::BitIdx_enableField2)) {
            field2Mode = comms::field::OptionalMode::Exists;
        }

        auto field3Mode = comms::field::OptionalMode::Missing;
        if (field_field1().getBitValue(Field1Type::BitIdx_enableField3)) {
            field3Mode = comms::field::OptionalMode::Exists;
        }

        field_field2().setMode(field2Mode);
        field_field3().setMode(field3Mode);

        return Base::template doReadFrom<FieldIdx_field2>(iter, len);
    }

    /// @brief Implement custom refresh functionality.
    /// @details Brings the message fields into a consistent state.
    /// @return @b true in case any of the optional fields has been updated,
    ///     @b false otherwise.
    bool doRefresh()
    {
        bool refreshed = Base::doFieldsVersionUpdate();
        using Field1Type = typename std::decay<decltype(field_field1())>::type;
        auto field2ExpectedMode = comms::field::OptionalMode::Missing;
        if (field_field1().getBitValue(Field1Type::BitIdx_enableField2)) {
            field2ExpectedMode = comms::field::OptionalMode::Exists;
        }

        auto field3ExpectedMode = comms::field::OptionalMode::Missing;
        if (field_field1().getBitValue(Field1Type::BitIdx_enableField3)) {
            field3ExpectedMode = comms::field::OptionalMode::Exists;
        }

        if (field_field2().getMode() != field2ExpectedMode) {
            field_field2().setMode(field2ExpectedMode);
            refreshed = true;
        }

        if (field_field3().getMode() != field3ExpectedMode) {
            field_field3().setMode(field3ExpectedMode);
            refreshed = true;
        }

        return refreshed;
    }

    /// @brief Name of the message.
    static const char* doName()
    {
        return "Optionals";
    }
};

}  // namespace message

}  // namespace demo


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
/// @brief Contains definition of Variants message and its fields.

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

/// @brief Accumulates details of all the Variants message fields.
/// @tparam TOpt Extra options
/// @see Variants
template <typename TOpt = demo::DefaultOptions>
struct VariantsFields
{
    /// @brief Enumerator vor @ref varIdField
    enum class VarId : std::uint8_t
    {
        Elem1, ///< Value to identify @ref field1_var1
        Elem2, ///< Value to identify @ref field1_var2
        Elem3, ///< Value to identify @ref field1_var3
        NumOfValues ///< number of available values
    };

    /// @brief field used to identify the value of the variant field @ref field1.
    template <VarId TId>
    class varIdField : public
        comms::field::EnumValue<
            demo::FieldBase,
            VarId,
            comms::option::def::DefaultNumValue<static_cast<int>(TId)>,
            comms::option::def::ValidNumValueRange<static_cast<int>(TId), static_cast<int>(TId)>,
            comms::option::def::FailOnInvalid<>,
            comms::option::def::FixedValue,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::EnumValue<
                demo::FieldBase,
                VarId,
                comms::option::def::DefaultNumValue<static_cast<int>(TId)>,
                comms::option::def::ValidNumValueRange<static_cast<int>(TId), static_cast<int>(TId)>,
                comms::option::def::FailOnInvalid<>,
                comms::option::def::FixedValue,
                comms::option::def::HasName
            >;                
    public:   
        using ValueType = typename Base::ValueType;
        using ValueNameInfo = const char*;
        using ValueNamesMapInfo = std::pair<const ValueNameInfo*, std::size_t>;

        static ValueType firstValue() 
        {
            return ValueType::Elem1;
        }

        static ValueType lastValue() 
        {
            return ValueType::Elem3;
        }        

        static ValueType valuesLimit() 
        {
            return ValueType::NumOfValues;
        }  

        static const char* name()
        {
            return "id";
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
                "Elem1",
                "Elem2",
                "Elem3",
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }               
    };

    /// @brief field used to identify the remaining length.
    class lengthField : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint8_t,
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
            return "length";
        }                 
    }; 

    struct field1_var1Members
    {
        class Value : public 
            comms::field::IntValue<
                demo::FieldBase,
                std::uint8_t,
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
                return "value";
            }
        };                    
    };

    /// @brief First type that can be stored in @ref field1 variant field.
    /// @details Identified by @ref VarId::Elem1 (0) value which is followed by
    ///     the 1 byte unsigned integer.
    class field1_var1 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                varIdField<VarId::Elem1>,
                typename field1_var1Members::Value
            >,
            typename TOpt::message::VariantsFields::field1_var1,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    varIdField<VarId::Elem1>,
                    typename field1_var1Members::Value
                >,
                typename TOpt::message::VariantsFields::field1_var1,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b id for @ref varIdField<VarId::Elem1>
        ///     @b value for 1 byte unsigned integer field.
        COMMS_FIELD_MEMBERS_NAMES(id, value);

        static const char* name()
        {
            return "field1_var1";
        }                 
    };

    struct field1_var2Members
    {
        class Value : public 
            comms::field::IntValue<
                demo::FieldBase,
                std::uint32_t,
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
                return "value";
            }
        };                    
    };    

    /// @brief Second type that can be stored in @ref field1 variant field.
    /// @details Identified by @ref VarId::Elem2 (1) value which is followed by
    ///     the 4 bytes unsigned integer.
    class field1_var2 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                varIdField<VarId::Elem2>,
                typename field1_var2Members::Value
            >,
            typename TOpt::message::VariantsFields::field1_var2,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    varIdField<VarId::Elem2>,
                    typename field1_var2Members::Value
                >,
                typename TOpt::message::VariantsFields::field1_var2,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b id for @ref varIdField<VarId::Elem1>
        ///     @b value for 4 bytes unsigned integer field.
        COMMS_FIELD_MEMBERS_NAMES(id, value);

        static const char* name()
        {
            return "field1_var2";
        }           
    };

    struct field1_var3Members
    {
        class Value : public 
            comms::field::String<
                demo::FieldBase,
                comms::option::def::HasName,
                comms::option::SequenceSizeFieldPrefix<
                    comms::field::IntValue<
                        demo::FieldBase,
                        std::uint8_t
                    >
                >
            >
        {
        public:
            static constexpr bool hasSpecials()
            {
                return false;
            }
                    
            static const char* name()
            {
                return "value";
            }
        };                    
    };      

    /// @brief Third type that can be stored in @ref field1 variant field.
    /// @details Identified by @ref VarId::Elem3 (2) value which is followed by
    ///     the string prefixed with its lengths (1 byte).
    class field1_var3 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                varIdField<VarId::Elem3>,
                typename field1_var3Members::Value
            >,
            typename TOpt::message::VariantsFields::field1_var3,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    varIdField<VarId::Elem3>,
                    typename field1_var3Members::Value
                >,
                typename TOpt::message::VariantsFields::field1_var3,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b id for @ref varIdField<VarId::Elem1>
        ///     @b value for string field prefixed with its size.
        COMMS_FIELD_MEMBERS_NAMES(id, value);

        static const char* name()
        {
            return "field1_var3";
        }        
    };

    /// @brief Variant field.
    /// @details Can be one of the following:
    ///     @li @ref field1_var1
    ///     @li @ref field1_var2
    ///     @li @ref field1_var3
    class field1 : public
        comms::field::Variant<
            demo::FieldBase,
            std::tuple<
                field1_var1,
                field1_var2,
                field1_var3
            >,
            typename TOpt::message::VariantsFields::field1,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Variant<
                demo::FieldBase,
                std::tuple<
                    field1_var1,
                    field1_var2,
                    field1_var3
                >,
                typename TOpt::message::VariantsFields::field1,
                comms::option::def::HasName
            >;
    public:
        using MemberNameInfo = const char*;
        using MemberNamesMapInfo = std::pair<const MemberNameInfo*, std::size_t>;
            
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_VARIANT_MEMBERS_NAMES macro
        ///     related to @b comms::field::Variant class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b val1 for @ref field1_var1
        ///     @b val2 for @ref field1_var2
        ///     @b val3 for @ref field1_var3
        COMMS_VARIANT_MEMBERS_NAMES(val1, val2, val3);

        static const char* name()
        {
            return "field1";
        }  

        static const char* memberName(std::size_t idx)
        {
            auto namesMapInfo = memberNamesMap();
            if (namesMapInfo.second <= idx) {
                return nullptr;
            }

            return namesMapInfo.first[idx];
        }        

        static MemberNamesMapInfo memberNamesMap()
        {
            static const MemberNameInfo Map[] = {
                field1_var1::name(),
                field1_var2::name(),
                field1_var3::name(),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;
            static_assert(MapSize == FieldIdx_numOfValues, "Invalid map");

            return std::make_pair(&Map[0], MapSize);
        }               
    };

    struct field2_var1Members
    {
        class Value : public 
            comms::field::IntValue<
                demo::FieldBase,
                std::uint8_t,
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
                return "value";
            }
        };                    
    };      

    /// @brief First type that can be stored in @ref field2 variant field.
    /// @details Identified by @ref VarId::Elem1 (0) value which is followed by
    ///     the length and 1 byte unsigned integer.
    class field2_var1 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                varIdField<VarId::Elem1>,
                lengthField,
                typename field2_var1Members::Value
            >,
            typename TOpt::message::VariantsFields::field2_var1,
            comms::option::def::RemLengthMemberField<1>,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    varIdField<VarId::Elem1>,
                    lengthField,
                    typename field2_var1Members::Value
                >,
                typename TOpt::message::VariantsFields::field2_var1,
                comms::option::def::RemLengthMemberField<1>,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b id for @ref varIdField<VarId::Elem1>
        ///     @b length for @ref lengthField.
        ///     @b value for 1 byte unsigned integer field.
        COMMS_FIELD_MEMBERS_NAMES(id, length, value);

        static const char* name()
        {
            return "field2_var1";
        }        
    };

    struct field2_var2Members
    {
        class Value : public 
            comms::field::IntValue<
                demo::FieldBase,
                std::uint32_t,
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
                return "value";
            }
        };                    
    };    

    /// @brief Second type that can be stored in @ref field2 variant field.
    /// @details Identified by @ref VarId::Elem2 (1) value which is followed by
    ///     the 4 bytes unsigned integer.
    class field2_var2 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                varIdField<VarId::Elem2>,
                lengthField,
                typename field2_var2Members::Value
            >,
            typename TOpt::message::VariantsFields::field2_var2,
            comms::option::def::RemLengthMemberField<1>,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    varIdField<VarId::Elem2>,
                    lengthField,
                    typename field2_var2Members::Value
                >,
                typename TOpt::message::VariantsFields::field2_var2,
                comms::option::def::RemLengthMemberField<1>,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b id for @ref varIdField<VarId::Elem1>
        ///     @b length for @ref lengthField.
        ///     @b value for 4 bytes unsigned integer field.
        COMMS_FIELD_MEMBERS_NAMES(id, length, value);

        static const char* name()
        {
            return "field2_var2";
        }        
    };

    struct field2_var3Members
    {
        class Value : public 
            comms::field::String<demo::FieldBase, comms::option::def::HasName>
        {
        public:
            static const char* name()
            {
                return "value";
            }
        };                    
    };     

    /// @brief Third type that can be stored in @ref field2 variant field.
    /// @details Identified by @ref VarId::Elem3 (2) value which is followed by
    ///     the string prefixed with its lengths (1 byte).
    class field2_var3 : public
        comms::field::Bundle<
            demo::FieldBase,
            std::tuple<
                varIdField<VarId::Elem3>,
                lengthField,
                typename field2_var3Members::Value
            >,
            comms::option::def::RemLengthMemberField<1>,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Bundle<
                demo::FieldBase,
                std::tuple<
                    varIdField<VarId::Elem3>,
                    lengthField,
                    typename field2_var3Members::Value
                >,
                comms::option::def::RemLengthMemberField<1>,
                comms::option::def::HasName
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_ACCESS_NOTEMPLATE macro
        ///     related to @b comms::field::Bundle class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b id for @ref varIdField<VarId::Elem1>
        ///     @b length for @ref lengthField.
        ///     @b value for string field.
        COMMS_FIELD_MEMBERS_NAMES(id, length, value);

        static const char* name()
        {
            return "field2_var3";
        }        
    };    

    /// @brief Variant field.
    /// @details Can be one of the following:
    ///     @li @ref field2_var1
    ///     @li @ref field2_var2
    ///     @li @ref field2_var3
    class field2 : public
        comms::field::Variant<
            demo::FieldBase,
            std::tuple<
                field2_var1,
                field2_var2,
                field2_var3
            >,
            typename TOpt::message::VariantsFields::field2,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::Variant<
                demo::FieldBase,
                std::tuple<
                    field2_var1,
                    field2_var2,
                    field2_var3
                >,
                typename TOpt::message::VariantsFields::field2,
                comms::option::def::HasName
            >;
    public:
        using MemberNameInfo = const char*;
        using MemberNamesMapInfo = std::pair<const MemberNameInfo*, std::size_t>;
            
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_VARIANT_MEMBERS_NAMES macro
        ///     related to @b comms::field::Variant class from COMMS library
        ///     for details. @n
        ///     The names are:
        ///     @b val1 for @ref field2_var1
        ///     @b val2 for @ref field2_var2
        ///     @b val3 for @ref field2_var3
        COMMS_VARIANT_MEMBERS_NAMES(val1, val2, val3);

        static const char* name()
        {
            return "field2";
        }  

        static const char* memberName(std::size_t idx)
        {
            auto namesMapInfo = memberNamesMap();
            if (namesMapInfo.second <= idx) {
                return nullptr;
            }

            return namesMapInfo.first[idx];
        }        

        static MemberNamesMapInfo memberNamesMap()
        {
            static const MemberNameInfo Map[] = {
                field2_var1::name(),
                field2_var2::name(),
                field2_var3::name(),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;
            static_assert(MapSize == FieldIdx_numOfValues, "Invalid map");

            return std::make_pair(&Map[0], MapSize);
        }               
    };    

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1,
        field2
    >;
};

/// @brief Accumulates various list fields.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref VariantsFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class Variants : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Variants,
        comms::option::StaticNumIdImpl<MsgId_Variants>,
        comms::option::FieldsImpl<typename VariantsFields<TOpt>::All>,
        comms::option::MsgType<Variants<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Variants,
            comms::option::StaticNumIdImpl<MsgId_Variants>,
            comms::option::FieldsImpl<typename VariantsFields<TOpt>::All>,
            comms::option::MsgType<Variants<TMsgBase, TOpt> >,
            comms::option::HasName
        >;

    static const bool AreFieldsVersionDependent = Base::areFieldsVersionDependent();
    static_assert(!AreFieldsVersionDependent, "Fields mustn't be version dependent");
public:

    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    COMMS_MSG_FIELDS_NAMES(field1, field2);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when call to doMinLength()
    // is performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static_assert(MsgMinLen == 0, "Unexpected min serialisation length");

    /// @brief Default constructor
    Variants() = default;

    /// @brief Copy constructor
    Variants(const Variants&) = default;

    /// @brief Move constructor
    Variants(Variants&& other) = default;

    /// @brief Destructor
    ~Variants() noexcept = default;

    /// @brief Copy assignment
    Variants& operator=(const Variants&) = default;

    /// @brief Move assignment
    Variants& operator=(Variants&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "Variants";
    }
};

}  // namespace message

}  // namespace demo



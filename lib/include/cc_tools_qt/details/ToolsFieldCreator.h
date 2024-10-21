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


#include <memory>

#include "comms/comms.h"

#include "cc_tools_qt/details/ToolsArrayListFieldImpl.h"
#include "cc_tools_qt/details/ToolsBitfieldFieldImpl.h"
#include "cc_tools_qt/details/ToolsBitmaskFieldImpl.h"
#include "cc_tools_qt/details/ToolsEnumFieldImpl.h"
#include "cc_tools_qt/details/ToolsFloatFieldImpl.h"
#include "cc_tools_qt/details/ToolsIntFieldImpl.h"
#include "cc_tools_qt/details/ToolsOptionalFieldImpl.h"
#include "cc_tools_qt/details/ToolsRawDataFieldImpl.h"
#include "cc_tools_qt/details/ToolsStringFieldImpl.h"
#include "cc_tools_qt/details/ToolsUnsignedLongFieldImpl.h"

#include "cc_tools_qt/field_wrapper/BundleWrapper.h"
#include "cc_tools_qt/field_wrapper/VariantWrapper.h"
#include "cc_tools_qt/field_wrapper/UnknownValueWrapper.h"

#include <vector>

namespace cc_tools_qt
{

namespace details
{

class ToolsFieldCreator
{
public:
    using ToolsFieldPtr = cc_tools_qt::ToolsFieldPtr;
    using FieldsList = std::vector<ToolsFieldPtr>;

    explicit ToolsFieldCreator(FieldsList& fields) : m_fields(fields) {}

    template <typename TField>
    static ToolsFieldPtr createField(TField& field)
    {
        using DecayedField = typename std::decay<decltype(field)>::type;
        using Tag = typename DecayedField::CommsTag;        
        return createFieldInternal(field, Tag());
    }

    template <typename TField>
    void operator()(TField& field)
    {
        m_fields.push_back(createField(field));
    }
private:
    using IntValueTag = comms::field::tag::Int;
    using BitmaskValueTag = comms::field::tag::Bitmask;
    using EnumValueTag = comms::field::tag::Enum;
    using StringTag = comms::field::tag::String;
    using BitfieldTag = comms::field::tag::Bitfield;
    using OptionalTag = comms::field::tag::Optional;
    using BundleTag = comms::field::tag::Bundle;
    using RawDataArrayListTag = comms::field::tag::RawArrayList;
    using FieldsArrayListTag = comms::field::tag::ArrayList;
    using FloatValueTag = comms::field::tag::Float;
    using VariantTag = comms::field::tag::Variant;

    struct RegularIntTag {};
    struct BigUnsignedTag {};

    class SubfieldsCreateHelper
    {
    public:
        typedef std::function <void (ToolsFieldPtr)> WrapperDispatchFunc;
        SubfieldsCreateHelper(WrapperDispatchFunc&& dispatchOp)
          : m_dispatchOp(std::move(dispatchOp))
        {
        }

        template <typename TField>
        void operator()(TField&& field)
        {
            auto fieldWidget =
                ToolsFieldCreator::createField(std::forward<TField>(field));
            m_dispatchOp(std::move(fieldWidget));
        }

        template <std::size_t TIdx, typename TField>
        void operator()(TField&& field)
        {
            return operator()(field);
        }

    private:
        WrapperDispatchFunc m_dispatchOp;
    };


    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, IntValueTag)
    {
        using FieldType = std::decay_t<decltype(field)>;
        using ValueType = typename FieldType::ValueType;

        static_assert(std::is_integral<ValueType>::value,
            "ValueType is expected to be integral");

        using Tag = 
            std::conditional_t<
                std::is_signed_v<ValueType> || (sizeof(ValueType) < sizeof(std::uint64_t)),
                RegularIntTag,
                BigUnsignedTag
            >;

        return createFieldInternal(field, Tag());
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, RegularIntTag)
    {
        return makeIntField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, BigUnsignedTag)
    {
        return makeUnsignedLongField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, BitmaskValueTag)
    {
        return makeBitmaskField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, EnumValueTag)
    {
        return makeEnumField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, StringTag)
    {
        return makeStringField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, BitfieldTag)
    {
        auto toolsField = makeBitfieldField(field);

        using FieldPtrType = std::decay_t<decltype(toolsField)>;
        using FieldType = typename FieldPtrType::element_type;
        using MembersListType = typename FieldType::Members;

        MembersListType subFields;
        auto& memberFields = field.value();
        comms::util::tupleForEach(
            memberFields,
            SubfieldsCreateHelper(
                [&subFields](ToolsFieldPtr fieldWrapper)
                {
                    subFields.push_back(std::move(fieldWrapper));
                }));

        toolsField->setMembers(std::move(subFields));
        return toolsField;
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, OptionalTag)
    {
        auto fieldPtr = makeOptionalField(field);
        auto& wrappedField = field.field();
        auto fieldWrapper = createField(wrappedField);
        fieldPtr->setFieldWrapper(std::move(fieldWrapper));
        return fieldPtr;
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, BundleTag)
    {
        auto wrapper = field_wrapper::makeBundleWrapper(field);

        typedef typename std::decay<decltype(wrapper)>::type WrapperPtrType;
        typedef typename WrapperPtrType::element_type WrapperType;
        typedef typename WrapperType::Members MembersWrappersList;

        MembersWrappersList subWrappers;
        auto& memberFields = field.value();
        comms::util::tupleForEach(
            memberFields,
            SubfieldsCreateHelper(
                [&subWrappers](ToolsFieldPtr fieldWrapper)
                {
                    subWrappers.push_back(std::move(fieldWrapper));
                }));

        wrapper->setMembers(std::move(subWrappers));
        return wrapper;
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, RawDataArrayListTag)
    {
        return makeRawDataField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, FieldsArrayListTag)
    {
        using DecayedField = std::decay_t<decltype(field)>;
        using CollectionType = typename DecayedField::ValueType;
        using ElementType = typename CollectionType::value_type;

        auto fieldPtr = makeArrayListField(field);
        if (fieldPtr->hasFixedSize()) {
            fieldPtr->adjustFixedSize();
        }

        fieldPtr->setWrapFieldCallback(
            [](ElementType& memField) -> ToolsFieldPtr
            {
                return ToolsFieldCreator::createField(memField);
            });

        fieldPtr->refreshMembers();
        return fieldPtr;
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, FloatValueTag)
    {
        return makeFloatField(field);
    }

    template <typename TField>
    static ToolsFieldPtr createFieldInternal(TField& field, VariantTag)
    {
        auto wrapper = field_wrapper::makeVariantWrapper(field);

        wrapper->setMemberCreateCallback(
            [&field]() -> ToolsFieldPtr
            {
                ToolsFieldPtr ptr;
                if (field.currentFieldValid()) {
                    field.currentFieldExec(
                        SubfieldsCreateHelper(
                            [&ptr](ToolsFieldPtr fieldWrapper) noexcept
                            {
                                ptr = std::move(fieldWrapper);
                            }));
                }
                return ptr;
            });

        if (field.currentFieldValid()) {
            field.currentFieldExec(
                SubfieldsCreateHelper(
                    [&wrapper](ToolsFieldPtr fieldWrapper)
                    {
                        wrapper->setCurrent(std::move(fieldWrapper));
                    }));
        }
        else {
            wrapper->setCurrent(ToolsFieldPtr());
        }

        return wrapper;
    }

    template <typename TField, typename TTag>
    static ToolsFieldPtr createFieldInternal(TField& field, TTag)
    {
        return field_wrapper::makeUnknownValueWrapper(field);
    }

private:
    FieldsList& m_fields;    
};

}  // namespace details

}  // namespace cc_tools_qt






//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/details/ToolsFieldBase.h"
#include "cc_tools_qt/field/ToolsArrayListField.h"

#include "comms/field/ArrayList.h"

#include <cstdint>
#include <cassert>
#include <memory>
#include <type_traits>
#include <functional>
#include <utility>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsArrayListFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsArrayListField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsArrayListField, TField>;
    using Field = TField;
    using ValueType = typename Field::ValueType;
    using ElementType = typename ValueType::value_type;

public:
    using SerialisedSeq = typename Base::SerialisedSeq;
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;
    using Members = typename Base::Members;

    using WrapFieldCallbackFunc = std::function<ToolsFieldPtr (ElementType&)>;

    explicit ToolsArrayListFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsArrayListFieldImpl(const ToolsArrayListFieldImpl&) = default;
    ToolsArrayListFieldImpl(ToolsArrayListFieldImpl&&) = default;
    virtual ~ToolsArrayListFieldImpl() noexcept = default;

    ToolsArrayListFieldImpl& operator=(const ToolsArrayListFieldImpl&) = delete;

    void setWrapFieldCallback(WrapFieldCallbackFunc&& func)
    {
        assert(func);
        m_wrapFieldFunc = std::move(func);
    }

protected:
    virtual bool isHiddenSerializationImpl() const override
    {
        static constexpr bool NoPrefix =
            (!Field::hasElemFixedSerLengthFieldPrefix()) &&
            (!Field::hasElemSerLengthFieldPrefix()) && 
            (!Field::hasSerLengthFieldPrefix()) &&
            (!Field::hasSizeFieldPrefix());
            
        static constexpr bool NoSuffix =
            (!Field::hasTerminationFieldSuffix()) &&
            (!Field::hasTrailingFieldSuffix());

        return NoPrefix && NoSuffix;
    }

    virtual void addFieldImpl() override
    {
        using Tag = 
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                HasFeatureTag
            >;

        addFieldInternal(Tag());
    }

    virtual void removeFieldImpl(int idx) override
    {
        using Tag = 
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                HasFeatureTag
            >;

        removeFieldInternal(idx, Tag());
    }

    virtual bool setSerialisedValueImpl([[maybe_unused]] const SerialisedSeq& value) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return false;
    }

    virtual unsigned sizeImpl() const override
    {
        return static_cast<unsigned>(Base::field().value().size());
    }

    virtual bool hasFixedSizeImpl() const override
    {
        return Field::hasFixedSize();
    }

    virtual void adjustFixedSizeImpl() override
    {
        using Tag =
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                std::conditional_t<
                    Field::hasFixedSize(),
                    HasFixedSizeTag,
                    HasVarSizeTag
                >
            >;
        adjustFixedSizeInternal(Tag());
    }

    virtual Ptr cloneImpl() override
    {
        ActPtr ptr(new ToolsArrayListFieldImpl(Base::field()));
        static_cast<ToolsArrayListFieldImpl<TField>*>(ptr.get())->m_wrapFieldFunc = m_wrapFieldFunc;
        return ptr;
    }

    virtual void refreshMembersImpl() override
    {
        using Tag = 
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                HasFeatureTag
            >;

        refreshMembersInternal(Tag());
    }

    virtual void membersUpdatedImpl() override
    {
        if (isHiddenSerializationImpl()) {
            return;
        }

        auto& mems = Base::getMembers();
        for (auto& m : mems) {
            assert(m);
            m->forceHiddenSerialization();
        }
    }

private:
    struct HasFixedSizeTag {};
    struct HasVarSizeTag {};
    struct HasFeatureTag {};
    struct NoFeatureTag {};     


    void adjustFixedSizeInternal(HasVarSizeTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }

    void adjustFixedSizeInternal(HasFixedSizeTag)
    {
        COMMS_GNU_WARNING_PUSH
#if defined(NDEBUG) && COMMS_IS_GCC_13 && (COMMS_IS_CPP20)
        COMMS_GNU_WARNING_DISABLE("-Wstringop-overflow")
#endif        

        Base::field().value().resize(Field::fixedSize());
        COMMS_GNU_WARNING_POP        
    }

    void adjustFixedSizeInternal(NoFeatureTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }    

    void addFieldInternal(HasFeatureTag)
    {
        auto& col = Base::field().value();
        col.push_back(ElementType());
        refreshMembersInternal(HasFeatureTag());
    }   

    void addFieldInternal(NoFeatureTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);        
    } 

    void removeFieldInternal(int idx, HasFeatureTag)
    {
        auto& storage = Base::field().value();
        if (static_cast<decltype(idx)>(storage.size()) <= idx) {
            return;
        }

        storage.erase(storage.begin() + idx);
        refreshMembersInternal(HasFeatureTag());
    }

    void removeFieldInternal([[maybe_unused]] int idx, NoFeatureTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);        
    }

    void refreshMembersInternal(HasFeatureTag)
    {
        if (!m_wrapFieldFunc) {
            [[maybe_unused]] static constexpr bool Callback_is_not_set = false;
            assert(Callback_is_not_set);  
        }

        auto& storage = Base::field().value();
        Members mems;
        mems.reserve(storage.size());
        for (auto& f : storage) {
            mems.push_back(m_wrapFieldFunc(f));
            if (!mems.back()->canWrite()) {
                mems.back()->reset();
                assert(mems.back()->canWrite());
            }
        }

        Base::setMembers(std::move(mems));
    }    

    void refreshMembersInternal(NoFeatureTag)
    {
    }    

    WrapFieldCallbackFunc m_wrapFieldFunc;
};


template <typename TField>
auto makeArrayListField(TField& field)
{
    return std::make_unique<ToolsArrayListFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt




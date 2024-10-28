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
    using PrefixFieldInfo = typename Base::PrefixFieldInfo;

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

    virtual PrefixFieldInfo getPrefixFieldInfoImpl() const override
    {
        using Tag =
            std::conditional_t<
                Field::hasSizeFieldPrefix(),
                ElemCountFieldTag,
                std::conditional_t<
                    Field::hasSerLengthFieldPrefix(),
                    SerLengthFieldTag,
                    NoPrefixFieldTag
                >
            >;

        return getPrefixFieldInfoInternal(Tag());
    }

private:
    struct NoPrefixFieldTag {};
    struct ElemCountFieldTag {};
    struct SerLengthFieldTag {};
    struct SerLengthFieldFixedTag {};
    struct SerLengthFieldVarTag {};
    struct HasFixedSizeTag {};
    struct HasVarSizeTag {};
    struct HasFeatureTag {};
    struct NoFeatureTag {};     

    PrefixFieldInfo getPrefixFieldInfoInternal(NoPrefixFieldTag) const
    {
        return std::make_pair(0, SerialisedSeq());
    }

    PrefixFieldInfo getPrefixFieldInfoInternal(ElemCountFieldTag) const
    {
        using SizeField = typename Field::SizeFieldPrefix;
        SizeField sizeField;
        sizeField.setValue(Base::field().value().size());
        return std::make_pair(static_cast<int>(sizeField.getValue()), getPrefixFieldSerialised(sizeField));
    }

    PrefixFieldInfo getPrefixFieldInfoInternal(SerLengthFieldTag) const
    {
        using LengthField = typename Field::SerLengthFieldPrefix;
        using Tag =
            std::conditional_t<
                LengthField::hasVarLength(),
                SerLengthFieldVarTag,
                SerLengthFieldFixedTag
            >;

        return getPrefixFieldInfoInternal(Tag());
    }

    PrefixFieldInfo getPrefixFieldInfoInternal(SerLengthFieldFixedTag) const
    {
        using LengthField = typename Field::SerLengthFieldPrefix;
        LengthField lenField;
        lenField.setValue(Base::field().length() - LengthField::maxLength());
        return std::make_pair(static_cast<int>(lenField.getValue()), getPrefixFieldSerialised(lenField));
    }

    PrefixFieldInfo getPrefixFieldInfoInternal(SerLengthFieldVarTag) const
    {
        using LengthField = typename Field::SerLengthFieldPrefix;

        auto fullLen = Base::field().length();
        LengthField lenFieldTmp;
        lenFieldTmp.setValue(fullLen);
        auto tmpLen = lenFieldTmp.length();
        LengthField lenField;
        lenField.setValue(fullLen - tmpLen);
        if (lenField.length() == tmpLen) {
            assert(static_cast<int>(lenField.getValue()) <= std::numeric_limits<int>::max());
            return std::make_pair(static_cast<int>(lenField.getValue()), getPrefixFieldSerialised(lenField));
        }

        lenField.setValue(fullLen - lenField.length());
        assert(static_cast<int>(lenField.getValue()) <= std::numeric_limits<int>::max());
        return std::make_pair(static_cast<int>(lenField.getValue()), getPrefixFieldSerialised(lenField));
    }

    template <typename TPrefixField>
    SerialisedSeq getPrefixFieldSerialised(const TPrefixField& prefixField) const
    {
        SerialisedSeq serData;
        serData.reserve(prefixField.length());
        auto writeIter = std::back_inserter(serData);
        [[maybe_unused]] auto es = prefixField.write(writeIter, serData.max_size());
        assert(es == comms::ErrorStatus::Success);
        return serData;
    }

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

        auto& mems = Base::getMembers();

        decltype(&col[0]) firstElemPtr = nullptr;
        if (!col.empty()) {
            firstElemPtr = &col[0];
        }

        col.push_back(ElementType());
        if (!m_wrapFieldFunc) {
            [[maybe_unused]] static constexpr bool Callback_is_not_set = false;
            assert(Callback_is_not_set);       
            mems.clear();
            return;
        }

        if (firstElemPtr == &col[0]) {
            mems.push_back(m_wrapFieldFunc(col.back()));
            assert(col.size() == mems.size());
            return;
        }

        mems.clear();
        mems.reserve(col.size());
        for (auto& f : col) {
            mems.push_back(m_wrapFieldFunc(f));
        }

        assert(col.size() == mems.size());
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
        auto& mems = Base::getMembers();
        mems.clear();
        mems.reserve(storage.size());
        for (auto& f : storage) {
            mems.push_back(m_wrapFieldFunc(f));
        }
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
        auto& mems = Base::getMembers();
        mems.clear();
        mems.reserve(storage.size());
        for (auto& f : storage) {
            mems.push_back(m_wrapFieldFunc(f));
            if (!mems.back()->canWrite()) {
                mems.back()->reset();
                assert(mems.back()->canWrite());
            }
        }
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




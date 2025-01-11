//
// Copyright 2017 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/details/ToolsNumericFieldImpl.h"
#include "cc_tools_qt/field/ToolsUnsignedLongField.h"

#include "comms/field/IntValue.h"

#include <cstdint>
#include <cassert>
#include <limits>
#include <memory>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsUnsignedLongFieldImpl : public ToolsNumericFieldImpl<cc_tools_qt::field::ToolsUnsignedLongField, TField>
{
    using Base = ToolsNumericFieldImpl<cc_tools_qt::field::ToolsUnsignedLongField, TField>;
    using Field = TField;
    static_assert(comms::field::isIntValue<Field>(), "Must be of IntValueField type");

public:
    using UnderlyingType = typename Base::UnderlyingType;
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;
    using SpecialsList = typename Base::SpecialsList;

    explicit ToolsUnsignedLongFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsUnsignedLongFieldImpl(const ToolsUnsignedLongFieldImpl&) = default;
    ToolsUnsignedLongFieldImpl(ToolsUnsignedLongFieldImpl&&) = default;
    virtual ~ToolsUnsignedLongFieldImpl() noexcept = default;

    ToolsUnsignedLongFieldImpl& operator=(const ToolsUnsignedLongFieldImpl&) = delete;

protected:
    virtual UnderlyingType minValueImpl() const override
    {
        return std::numeric_limits<typename Field::ValueType>::min();
    }

    virtual UnderlyingType maxValueImpl() const override
    {
        return std::numeric_limits<typename Field::ValueType>::max();
    }

    virtual double getScaledImpl() const override
    {
        return Base::field().template scaleAs<double>();
    }

    virtual void setScaledImpl(double value) override
    {
        Base::field().setScaled(value);
    }

    virtual double scaleValueImpl(UnderlyingType value) const override
    {
        Field fieldTmp;
        fieldTmp.setValue(value);
        return fieldTmp.template scaleAs<double>();
    }

    virtual bool isSignedImpl() const override
    {
        return std::is_signed<typename Field::ValueType>::value;
    }

    virtual std::size_t valueTypeSizeImpl() const override
    {
        return sizeof(typename Field::ValueType);
    }

    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsUnsignedLongFieldImpl<TField>(Base::field()));
    }

    virtual const SpecialsList& specialsImpl() const override
    {
        using Tag = 
            std::conditional_t<
                Field::hasSpecials(),
                HasFeatureTag,
                NoFeatureTag
            >;

        return specialsInternal(Tag());
    }

    virtual int scaledDecimalsImpl() const override
    {
        using Tag = 
            std::conditional_t<
                Field::hasScaling(),
                HasFeatureTag,
                NoFeatureTag
            >;

        return scaledDecimalsInternal(Tag());        
    }   

    virtual UnderlyingType getDisplayValueImpl() const override
    {
        return static_cast<UnderlyingType>(Base::field().getDisplayValue());
    }

    virtual void setDisplayValueImpl(UnderlyingType value) override
    {
        using Tag = 
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                HasFeatureTag
            >;

        setDisplayValueInternal(value, Tag());        
    }       

private:
    struct HasFeatureTag{};    
    struct NoFeatureTag{};

    static const SpecialsList& specialsInternal(HasFeatureTag)
    {
        static const SpecialsList List = createSpecialsList();
        return List;
    }

    static const SpecialsList& specialsInternal(NoFeatureTag)
    {
        static const SpecialsList List;
        return List;
    }  

    static int scaledDecimalsInternal(HasFeatureTag)
    {
        return Field::displayDecimals();
    }

    static int scaledDecimalsInternal(NoFeatureTag)
    {
        return 0;
    }     

    static SpecialsList createSpecialsList()
    {
        SpecialsList result;
        auto mapInfo = Field::specialNamesMap();
        for (auto idx = 0U; idx < mapInfo.second; ++idx) {
            auto& sInfo = mapInfo.first[idx];
            result.append(qMakePair(sInfo.second, static_cast<UnderlyingType>(sInfo.first)));
        }

        return result;
    }

    void setDisplayValueInternal(UnderlyingType value, HasFeatureTag)
    {
        Base::field().setDisplayValue(value);
    }   

    void setDisplayValueInternal([[maybe_unused]] UnderlyingType value, NoFeatureTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);
    }     
};

template <typename TField>
auto makeUnsignedLongField(TField& field)
{
    return std::make_unique<ToolsUnsignedLongFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

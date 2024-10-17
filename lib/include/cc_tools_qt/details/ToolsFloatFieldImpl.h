//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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
#include "cc_tools_qt/field/ToolsFloatField.h"

#include "comms/field/FloatValue.h"

#include <cstdint>
#include <cassert>
#include <cmath>
#include <limits>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsFloatFieldImpl : public ToolsNumericFieldImpl<cc_tools_qt::field::ToolsFloatField, TField>
{
    using Base = ToolsNumericFieldImpl<cc_tools_qt::field::ToolsFloatField, TField>;
    using Field = TField;
    static_assert(comms::field::isFloatValue<Field>(), "Must be of FloatValueField type");

public:

    typedef typename Base::UnderlyingType UnderlyingType;
    typedef typename Base::ActPtr ActPtr;

    explicit ToolsFloatFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsFloatFieldImpl(const ToolsFloatFieldImpl&) = default;
    ToolsFloatFieldImpl(ToolsFloatFieldImpl&&) = default;
    virtual ~ToolsFloatFieldImpl() noexcept = default;

    ToolsFloatFieldImpl& operator=(const ToolsFloatFieldImpl&) = delete;

protected:
    virtual ActPtr cloneImpl() override
    {
        return ActPtr(new ToolsFloatFieldImpl<TField>(Base::field()));
    }

    virtual bool isNanImpl() const override
    {
        return std::isnan(Base::field().getValue());
    }

    virtual void setNanImpl() override
    {
        Base::field().setValue(std::numeric_limits<typename TField::ValueType>::quiet_NaN());
    }

    virtual bool isInfImpl() const override
    {
        return std::isinf(Base::field().getValue()) && (0 < Base::field().getValue());
    }

    virtual void setInfImpl() override
    {
        Base::field().setValue(std::numeric_limits<typename TField::ValueType>::infinity());
    }

    virtual bool isMinusInfImpl() const override
    {
        return std::isinf(Base::field().getValue()) && (Base::field().getValue() < 0);
    }

    virtual void setMinusInfImpl() override
    {
        Base::field().setValue(-std::numeric_limits<typename TField::ValueType>::infinity());
    }

    virtual double getEpsilonImpl() const override
    {
        return static_cast<double>(std::numeric_limits<typename TField::ValueType>::epsilon());
    }
};

template <typename TField>
cc_tools_qt::field::ToolsFloatFieldPtr makeFloatField(TField& field)
{
    return std::make_unique<ToolsFloatFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

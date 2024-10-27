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

#include "cc_tools_qt/details/ToolsFieldBase.h"
#include "cc_tools_qt/details/ToolsNumericFieldBase.h"

#include <cstddef>
#include <type_traits>

namespace cc_tools_qt
{

namespace details
{

template <typename TBase, typename TField>
class ToolsNumericFieldImpl : public ToolsFieldBase<TBase, TField>
{
    using Base = ToolsFieldBase<TBase, TField>;

public:
    using UnderlyingType = typename Base::UnderlyingType;
    using SerialisedSeq = typename Base::SerialisedSeq;

protected:
    using Field = TField;
    using ValueType = typename Field::ValueType;

    static_assert(sizeof(ValueType) <= sizeof(UnderlyingType), "This wrapper cannot handle provided field.");
//    static_assert(
//        std::is_signed<ValueType>::value == std::is_signed<UnderlyingType>::value ||
//            (sizeof(ValueType) < sizeof(UnderlyingType)),
//        "This wrapper cannot handle provided field.");

    explicit ToolsNumericFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
        static_assert(
            std::is_base_of_v<ToolsNumericFieldBase<UnderlyingType>, ToolsNumericFieldImpl<TBase, TField>>,
            "Must inherit from NumericValueWrapper");
    }

    ToolsNumericFieldImpl(const ToolsNumericFieldImpl&) = default;
    ToolsNumericFieldImpl(ToolsNumericFieldImpl&&) = default;

    ToolsNumericFieldImpl& operator=(const ToolsNumericFieldImpl&) = delete;

    virtual ~ToolsNumericFieldImpl() noexcept = default;

    virtual UnderlyingType getValueImpl() const override
    {
        return static_cast<UnderlyingType>(Base::field().getValue());
    }

    virtual void setValueImpl(UnderlyingType value) override
    {
        Base::field().setValue(value);
    }

    virtual std::size_t minLengthImpl() const override
    {
        return Base::field().minLength();
    }

    virtual std::size_t maxLengthImpl() const override
    {
        return Base::field().maxLength();
    }

};

}  // namespace details

}  // namespace cc_tools_qt

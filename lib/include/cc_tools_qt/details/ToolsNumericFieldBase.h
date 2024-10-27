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

#include "cc_tools_qt/ToolsField.h"

#include <cstddef>
#include <type_traits>

namespace cc_tools_qt
{

namespace details
{

template <typename TUnderlyingType>
class ToolsNumericFieldBase : public ToolsField
{
    static_assert(std::is_integral<TUnderlyingType>::value || std::is_floating_point<TUnderlyingType>::value,
        "Underlying type is expected to be integral or floating point.");
    using Base = ToolsField;

public:
    using UnderlyingType = TUnderlyingType;
    using SerialisedSeq = Base::SerialisedSeq;

    ToolsNumericFieldBase() {}

    virtual ~ToolsNumericFieldBase() noexcept = default;

    UnderlyingType getValue() const
    {
        return getValueImpl();
    }

    void setValue(UnderlyingType value)
    {
        setValueImpl(value);
    }

    std::size_t minLength() const
    {
        return minLengthImpl();
    }

    std::size_t maxLength() const
    {
        return maxLengthImpl();
    }

    int minWidth() const
    {
        return static_cast<int>(minLength()) * 2;
    }

    int maxWidth() const
    {
        return static_cast<int>(maxLength()) * 2;
    }
    

private:
    virtual UnderlyingType getValueImpl() const = 0;
    virtual void setValueImpl(UnderlyingType value) = 0;
    virtual std::size_t minLengthImpl() const = 0;
    virtual std::size_t maxLengthImpl() const = 0;
};

}  // namespace details

}  // namespace cc_tools_qt

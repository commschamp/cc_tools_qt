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

#include "cc_tools_qt/field/ToolsIntField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsIntField::ToolsIntField() {}

ToolsIntField::~ToolsIntField() noexcept = default;

ToolsIntField::UnderlyingType ToolsIntField::minValue() const
{
    return minValueImpl();
}

ToolsIntField::UnderlyingType ToolsIntField::maxValue() const
{
    return maxValueImpl();
}

double ToolsIntField::getScaled() const
{
    return getScaledImpl();
}

void ToolsIntField::setScaled(double value)
{
    setScaledImpl(value);
}

double ToolsIntField::scaleValue(UnderlyingType value) const
{
    return scaleValueImpl(value);
}

bool ToolsIntField::isSigned() const
{
    return isSignedImpl();
}

std::size_t ToolsIntField::valueTypeSize() const
{
    return valueTypeSizeImpl();
}

const ToolsIntField::SpecialsList& ToolsIntField::specials() const
{
    return specialsImpl();
}

bool ToolsIntField::hasScaledDecimals() const
{
    return 0 < scaledDecimals();
}

int ToolsIntField::scaledDecimals() const
{
    return scaledDecimalsImpl();
}

void ToolsIntField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

ToolsIntField::UnderlyingType ToolsIntField::getDisplayValue() const
{
    return getDisplayValueImpl();
}

ToolsIntField::ActPtr ToolsIntField::actClone()
{
    return ActPtr(static_cast<ToolsIntField*>(clone().release()));
}

void ToolsIntField::setDisplayValue(UnderlyingType value)
{
    setDisplayValueImpl(value);
}

}  // namespace field

}  // namespace cc_tools_qt



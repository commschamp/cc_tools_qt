//
// Copyright 2017 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/field/ToolsUnsignedLongField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsUnsignedLongField::ToolsUnsignedLongField() {}

ToolsUnsignedLongField::~ToolsUnsignedLongField() noexcept = default;

ToolsUnsignedLongField::UnderlyingType ToolsUnsignedLongField::minValue() const
{
    return minValueImpl();
}

ToolsUnsignedLongField::UnderlyingType ToolsUnsignedLongField::maxValue() const
{
    return maxValueImpl();
}

double ToolsUnsignedLongField::getScaled() const
{
    return getScaledImpl();
}

void ToolsUnsignedLongField::setScaled(double value)
{
    setScaledImpl(value);
}

double ToolsUnsignedLongField::scaleValue(UnderlyingType value) const
{
    return scaleValueImpl(value);
}

bool ToolsUnsignedLongField::isSigned() const
{
    return isSignedImpl();
}

std::size_t ToolsUnsignedLongField::valueTypeSize() const
{
    return valueTypeSizeImpl();
}

ToolsUnsignedLongField::ActPtr ToolsUnsignedLongField::clone()
{
    return cloneImpl();
}

void ToolsUnsignedLongField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



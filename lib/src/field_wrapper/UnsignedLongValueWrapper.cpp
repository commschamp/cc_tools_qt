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

#include "cc_tools_qt/field_wrapper/UnsignedLongValueWrapper.h"

#include "cc_tools_qt/field_wrapper/FieldWrapperHandler.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

UnsignedLongValueWrapper::UnsignedLongValueWrapper() {}

UnsignedLongValueWrapper::~UnsignedLongValueWrapper() noexcept = default;

UnsignedLongValueWrapper::UnderlyingType UnsignedLongValueWrapper::minValue() const
{
    return minValueImpl();
}

UnsignedLongValueWrapper::UnderlyingType UnsignedLongValueWrapper::maxValue() const
{
    return maxValueImpl();
}

double UnsignedLongValueWrapper::getScaled() const
{
    return getScaledImpl();
}

void UnsignedLongValueWrapper::setScaled(double value)
{
    setScaledImpl(value);
}

double UnsignedLongValueWrapper::scaleValue(UnderlyingType value) const
{
    return scaleValueImpl(value);
}

bool UnsignedLongValueWrapper::isSigned() const
{
    return isSignedImpl();
}

std::size_t UnsignedLongValueWrapper::valueTypeSize() const
{
    return valueTypeSizeImpl();
}

UnsignedLongValueWrapper::Ptr UnsignedLongValueWrapper::clone()
{
    return cloneImpl();
}

void UnsignedLongValueWrapper::dispatchImpl(FieldWrapperHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field_wrapper

}  // namespace cc_tools_qt



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

#include "cc_tools_qt/field_wrapper/EnumValueWrapper.h"

#include "cc_tools_qt/field_wrapper/FieldWrapperHandler.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

EnumValueWrapper::EnumValueWrapper() {}

EnumValueWrapper::~EnumValueWrapper() noexcept = default;

EnumValueWrapper::ActPtr EnumValueWrapper::clone()
{
    return cloneImpl();
}

void EnumValueWrapper::dispatchImpl(FieldWrapperHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field_wrapper

}  // namespace cc_tools_qt



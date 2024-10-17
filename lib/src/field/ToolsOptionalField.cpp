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

#include "cc_tools_qt/field/ToolsOptionalField.h"

#include "cc_tools_qt/field_wrapper/FieldWrapperHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsOptionalField::ToolsOptionalField() = default;
ToolsOptionalField::~ToolsOptionalField() noexcept = default;

ToolsOptionalField::Mode ToolsOptionalField::getMode() const
{
    return getModeImpl();
}

void ToolsOptionalField::setMode(Mode mode) {
    setModeImpl(mode);
}

bool ToolsOptionalField::hasFieldWrapper() const
{
    return static_cast<bool>(m_fieldWrapper);
}

ToolsField& ToolsOptionalField::getFieldWrapper()
{
    assert(hasFieldWrapper());
    return *m_fieldWrapper;
}

const ToolsField& ToolsOptionalField::getFieldWrapper() const
{
    assert(hasFieldWrapper());
    return *m_fieldWrapper;
}

void ToolsOptionalField::setFieldWrapper(ToolsFieldPtr fieldWrapper)
{
    m_fieldWrapper = std::move(fieldWrapper);
}

ToolsOptionalField::ActPtr ToolsOptionalField::clone()
{
    auto ptr = cloneImpl();
    ptr->setFieldWrapper(m_fieldWrapper->upClone());
    return ptr;
}

void ToolsOptionalField::dispatchImpl(field_wrapper::FieldWrapperHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



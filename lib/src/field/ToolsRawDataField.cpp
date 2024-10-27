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

#include "cc_tools_qt/field/ToolsRawDataField.h"

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

namespace field
{

ToolsRawDataField::ToolsRawDataField() {}

ToolsRawDataField::~ToolsRawDataField() noexcept = default;

QString ToolsRawDataField::getValue() const
{
    return getValueImpl();
}

void ToolsRawDataField::setValue(const QString& val)
{
    setValueImpl(val);
}

int ToolsRawDataField::maxSize() const
{
    return maxSizeImpl();
}

int ToolsRawDataField::minSize() const
{
    return minSizeImpl();
}

bool ToolsRawDataField::getForcedShowAll() const
{
    return m_forcedShowAll;
}

void ToolsRawDataField::setForcedShowAll(bool val)
{
    m_forcedShowAll = val;
}

bool ToolsRawDataField::isTruncated() const
{
    if (m_forcedShowAll) {
        return false;
    }

    return TruncateLength < length();
}

ToolsRawDataField::ActPtr ToolsRawDataField::actClone()
{
    return ActPtr(static_cast<ToolsRawDataField*>(clone().release()));
}

void ToolsRawDataField::dispatchImpl(ToolsFieldHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field

}  // namespace cc_tools_qt



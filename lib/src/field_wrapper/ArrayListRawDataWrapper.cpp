//
// Copyright 2016 - 2023 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/field_wrapper/ArrayListRawDataWrapper.h"

#include "cc_tools_qt/field_wrapper/FieldWrapperHandler.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

ArrayListRawDataWrapper::ArrayListRawDataWrapper() {}

ArrayListRawDataWrapper::~ArrayListRawDataWrapper() noexcept = default;

QString ArrayListRawDataWrapper::getValue() const
{
    return getValueImpl();
}

void ArrayListRawDataWrapper::setValue(const QString& val)
{
    setValueImpl(val);
}

int ArrayListRawDataWrapper::maxSize() const
{
    return maxSizeImpl();
}

int ArrayListRawDataWrapper::minSize() const
{
    return minSizeImpl();
}

ArrayListRawDataWrapper::Ptr ArrayListRawDataWrapper::clone()
{
    return cloneImpl();
}

bool ArrayListRawDataWrapper::getForcedShowAll() const
{
    return m_forcedShowAll;
}

void ArrayListRawDataWrapper::setForcedShowAll(bool val)
{
    m_forcedShowAll = val;
}

bool ArrayListRawDataWrapper::isTruncated() const
{
    if (m_forcedShowAll) {
        return false;
    }

    return TruncateLength < length();
}

void ArrayListRawDataWrapper::dispatchImpl(FieldWrapperHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field_wrapper

}  // namespace cc_tools_qt



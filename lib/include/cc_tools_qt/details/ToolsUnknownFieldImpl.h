//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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
#include "cc_tools_qt/field/ToolsUnknownField.h"

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsUnknownFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsUnknownField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsUnknownField, TField>;
    using Field = TField;

public:
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsUnknownFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsUnknownFieldImpl(const ToolsUnknownFieldImpl&) = default;
    ToolsUnknownFieldImpl(ToolsUnknownFieldImpl&&) = default;
    virtual ~ToolsUnknownFieldImpl() noexcept = default;

    ToolsUnknownFieldImpl& operator=(const ToolsUnknownFieldImpl&) = delete;

protected:
    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsUnknownFieldImpl<TField>(Base::field()));
    }
};

template <typename TField>
auto makeUnknownField(TField& field)
{
    return std::make_unique<ToolsUnknownFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

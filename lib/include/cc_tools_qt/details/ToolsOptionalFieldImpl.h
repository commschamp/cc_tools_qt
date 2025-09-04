//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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
#include "cc_tools_qt/field/ToolsOptionalField.h"

#include <memory>
#include <cassert>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsOptionalFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsOptionalField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsOptionalField, TField>;
public:

    using Mode = typename Base::Mode;
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;
    using Field = TField;

    explicit ToolsOptionalFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

protected:
    virtual Mode getModeImpl() const override
    {
        return Base::field().getMode();
    }

    virtual void setModeImpl(Mode mode) override
    {
        Base::field().setMode(mode);
    }

    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsOptionalFieldImpl<TField>(Base::field()));
    }
};

template <typename TField>
auto makeOptionalField(TField& field)
{
    return std::make_unique<ToolsOptionalFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt



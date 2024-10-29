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

#include "cc_tools_qt/details/ToolsFieldBase.h"
#include "cc_tools_qt/field/ToolsBitfieldField.h"

#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>

#include "comms/field/Bitfield.h"

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsBitfieldFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsBitfieldField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsBitfieldField, TField>;

    using Field = TField;
    static_assert(comms::field::isBitfield<Field>(), "Must be of Bitfield field type");

public:
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsBitfieldFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsBitfieldFieldImpl(const ToolsBitfieldFieldImpl&) = default;
    ToolsBitfieldFieldImpl(ToolsBitfieldFieldImpl&&) = default;
    virtual ~ToolsBitfieldFieldImpl() noexcept = default;

    ToolsBitfieldFieldImpl& operator=(const ToolsBitfieldFieldImpl&) = delete;

protected:
    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsBitfieldFieldImpl<TField>(Base::field()));
    }

    virtual void membersUpdatedImpl() override
    {
        auto& mems = Base::getMembers();
        for (auto& m : mems) {
            assert(m);
            m->forceHiddenSerialization();
        }
    }

};

template <typename TField>
auto makeBitfieldField(TField& field)
{
    return std::make_unique<ToolsBitfieldFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

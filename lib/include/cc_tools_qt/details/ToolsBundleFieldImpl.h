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
#include "cc_tools_qt/field/ToolsBundleField.h"

#include "comms/field/Bundle.h"

#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsBundleFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsBundleField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsBundleField, TField>;
    using Field = TField;

    static_assert(comms::field::isBundle<Field>(), "Must be of Bundle field type");

public:
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsBundleFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsBundleFieldImpl(const ToolsBundleFieldImpl&) = default;
    ToolsBundleFieldImpl(ToolsBundleFieldImpl&&) = default;
    virtual ~ToolsBundleFieldImpl() noexcept = default;

    ToolsBundleFieldImpl& operator=(const ToolsBundleFieldImpl&) = delete;

protected:
    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsBundleFieldImpl(Base::field()));
    }

    virtual bool isHiddenSerializationImpl() const override
    {
        return true;
    }
};

template <typename TField>
auto makeBundleField(TField& field)
{
    return std::make_unique<ToolsBundleFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

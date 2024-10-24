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

#include "cc_tools_qt/details/ToolsNumericFieldImpl.h"
#include "cc_tools_qt/field/ToolsEnumField.h"

#include "comms/field/EnumValue.h"

#include <cstdint>
#include <cassert>
#include <limits>
#include <memory>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsEnumFieldImpl : public ToolsNumericFieldImpl<cc_tools_qt::field::ToolsEnumField, TField>
{
    using Base = ToolsNumericFieldImpl<cc_tools_qt::field::ToolsEnumField, TField>;
    using Field = TField;

    static_assert(comms::field::isEnumValue<Field>(), "Must be of EnumValueField type");

    using ValueType = typename Field::ValueType;
    using UnderlyingType = typename Base::UnderlyingType;
    static_assert(sizeof(ValueType) <= sizeof(UnderlyingType), "This wrapper cannot handle provided field.");
//    static_assert(std::is_signed<ValueType>::value || (sizeof(ValueType) < sizeof(UnderlyingType)),
//        "This wrapper cannot handle provided field.");

public:
    using ValueInfosList = typename Base::ValueInfosList;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsEnumFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsEnumFieldImpl(const ToolsEnumFieldImpl&) = default;
    ToolsEnumFieldImpl(ToolsEnumFieldImpl&&) = default;
    virtual ~ToolsEnumFieldImpl() noexcept = default;

    ToolsEnumFieldImpl& operator=(const ToolsEnumFieldImpl&) = delete;

protected:
    virtual ActPtr cloneImpl() override
    {
        return ActPtr(new ToolsEnumFieldImpl<TField>(Base::field()));
    }

    const ValueInfosList& valuesImpl() const override
    {
        using Tag = 
            std::conditional_t<
                std::is_same_v<typename Field::ValueNameInfo, const char*>,
                SeqValuesTag,
                SparseValuesTag
            >;

        static const ValueInfosList List(createValues(Tag()));
        return List;
    }

private:
    struct SeqValuesTag{};
    struct SparseValuesTag{};

    static ValueInfosList createValues(SeqValuesTag)
    {
        ValueInfosList result;
        auto namesMap = Field::valueNamesMap();
        for (auto idx = 0U; idx < namesMap.second; ++idx) {
            auto* name = namesMap.first[idx];
            if (name == nullptr) {
                continue;
            }

            result.append(qMakePair(name, static_cast<long long>(idx)));
        }
        return result;
    }

    static ValueInfosList createValues(SparseValuesTag)
    {
        ValueInfosList result;
        auto namesMap = Field::valueNamesMap();
        for (auto idx = 0U; idx < namesMap.second; ++idx) {
            auto& info = namesMap.first[idx];
            result.append(qMakePair(info.second, static_cast<long long>(info.first)));
        }
        return result;
    }
};

template <typename TField>
auto makeEnumField(TField& field)
{
    return std::make_unique<ToolsEnumFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

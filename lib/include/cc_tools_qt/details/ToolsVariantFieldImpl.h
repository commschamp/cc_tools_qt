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


#pragma once

#include "cc_tools_qt/details/ToolsFieldBase.h"
#include "cc_tools_qt/field/ToolsVariantField.h"

#include "comms/field/Variant.h"

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include <string>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsVariantFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsVariantField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsVariantField, TField>;
    using Field = TField;
    static_assert(comms::field::isVariant<Field>(), "Must be of Variant field type");

public:
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsVariantFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsVariantFieldImpl(const ToolsVariantFieldImpl&) = default;
    ToolsVariantFieldImpl(ToolsVariantFieldImpl&&) = default;
    virtual ~ToolsVariantFieldImpl() noexcept = default;

    ToolsVariantFieldImpl& operator=(const ToolsVariantFieldImpl&) = delete;

protected:
    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsVariantFieldImpl(Base::field()));
    }

    virtual int getCurrentIndexImpl() const override
    {
        if (!Base::field().currentFieldValid()) {
            return -1;
        }

        return static_cast<int>(Base::field().currentField());
    }

    virtual void setCurrentIndexImpl(int index) override
    {
        if (index < 0) {
            Base::field().reset();
            return;
        }

        Base::field().selectField(static_cast<std::size_t>(index));
    }

    virtual int getMembersCountImpl() const override
    {
        return
            static_cast<int>(
                std::tuple_size<typename Base::Field::Members>::value);
    }

    virtual QStringList membersNamesImpl() const override
    {
        QStringList names;
        comms::util::tupleForEachType<typename Field::Members>(MembersNameHelper(names));
        return names;
    }  

private:
    class MembersNameHelper
    {
    public:
        explicit MembersNameHelper(QStringList& names) : m_names(names) {}

        template <typename TFieldType>
        void operator()()
        {
            m_names.append(TFieldType::name());
        }

    private:
        QStringList& m_names;        
    };

};

template <typename TField>
auto makeVariantField(TField& field)
{
    return std::make_unique<ToolsVariantFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt

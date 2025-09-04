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
#include "cc_tools_qt/field/ToolsStringField.h"

#include "comms/field/String.h"

#include <cstdint>
#include <cassert>
#include <limits>
#include <memory>
#include <type_traits>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsStringFieldImpl : public ToolsFieldBase<cc_tools_qt::field::ToolsStringField, TField>
{
    using Base = ToolsFieldBase<cc_tools_qt::field::ToolsStringField, TField>;
    using Field = TField;

public:
    using SerialisedSeq = typename Base::SerialisedSeq;
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsStringFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsStringFieldImpl(const ToolsStringFieldImpl&) = default;
    ToolsStringFieldImpl(ToolsStringFieldImpl&&) = default;
    virtual ~ToolsStringFieldImpl() noexcept = default;

    ToolsStringFieldImpl& operator=(const ToolsStringFieldImpl&) = delete;

protected:

    virtual QString getValueImpl() const override
    {
        auto& strField = Base::field();
        return QString::fromUtf8(strField.getValue().c_str(), static_cast<int>(strField.getValue().size()));
    }

    virtual void setValueImpl(const QString& val) override
    {
        using Tag =
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                HasFeatureTag
            >;

        setValueInternal(val, Tag());
    }

    virtual bool setSerialisedValueImpl([[maybe_unused]] const SerialisedSeq& value) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);
        return false;
    }

    virtual int maxSizeImpl() const override
    {
        return maxSizeInternal(SizeExistanceTag());
    }

    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsStringFieldImpl<TField>(Base::field()));
    }

private:
    struct SizeFieldExistsTag {};
    struct SerLengthFieldExistsTag {};
    struct NoSizeFieldTag {};
    struct HasFeatureTag {};
    struct NoFeatureTag {};

    using SizeExistanceTag =
        std::conditional_t<
            Field::hasSizeFieldPrefix(),
            SizeFieldExistsTag,
            std::conditional_t<
                Field::hasSerLengthFieldPrefix(),
                SerLengthFieldExistsTag,
                NoSizeFieldTag
            >
        >;

    template <typename TPrefixField>
    static int maxSizeByPrefix()
    {
        if (sizeof(int) <= TPrefixField::maxLength()) {
            return std::numeric_limits<int>::max();
        }

        auto shift =
            TPrefixField::maxLength() * std::numeric_limits<std::uint8_t>::digits;

        return static_cast<int>((1U << shift) - 1);
    }

    static int maxSizeInternal(SizeFieldExistsTag)
    {
        using SizeField = typename Field::SizeFieldPrefix;
        return maxSizeByPrefix<SizeField>();
    }

    static int maxSizeInternal(SerLengthFieldExistsTag)
    {
        using LengthField = typename Field::SerLengthFieldPrefix;
        return maxSizeByPrefix<LengthField>();
    }

    int maxSizeInternal(NoSizeFieldTag) const
    {
        return
            static_cast<int>(
                std::min(
                    static_cast<std::size_t>(std::numeric_limits<int>::max()),
                    Base::field().getValue().max_size()));
    }

    void setValueInternal(const QString& val, HasFeatureTag)
    {
        Base::field().setValue(val.toStdString().c_str());
    }

    void setValueInternal([[maybe_unused]] const QString& val, NoFeatureTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);
    }
};

template <typename TField>
auto makeStringField(TField& field)
{
    return std::make_unique<ToolsStringFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt


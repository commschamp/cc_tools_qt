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

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <memory>
#include <vector>

#include <QtCore/QString>

#include "comms/comms.h"
#include "cc_tools_qt/Api.h"

COMMS_MSVC_WARNING_PUSH
COMMS_MSVC_WARNING_DISABLE(4127) // Disable warning about constant conditional expressions

namespace cc_tools_qt
{

namespace field_wrapper
{

class FieldWrapperHandler;

/// @brief Field wrapping class.
class CC_API FieldWrapper
{
public:
    typedef std::vector<std::uint8_t> SerialisedSeq;

    typedef std::unique_ptr<FieldWrapper> BasePtr;

    FieldWrapper();
    virtual ~FieldWrapper() noexcept;

    std::size_t length() const;

    int width() const;

    bool valid() const;

    SerialisedSeq getSerialisedValue() const;

    bool setSerialisedValue(const SerialisedSeq& value);

    QString getSerialisedString() const;

    bool setSerialisedString(const QString& str);

    void dispatch(FieldWrapperHandler& handler);

    BasePtr upClone();

    bool canWrite() const;

    void reset();

protected:
    virtual std::size_t lengthImpl() const = 0;
    virtual bool validImpl() const = 0;
    virtual SerialisedSeq getSerialisedValueImpl() const = 0;
    virtual bool setSerialisedValueImpl(const SerialisedSeq& value) = 0;
    virtual void dispatchImpl(FieldWrapperHandler& handler) = 0;
    virtual BasePtr upCloneImpl() = 0;
    virtual bool canWriteImpl() const = 0;
    virtual void resetImpl() = 0;
};

template <typename TBase, typename TField>
class FieldWrapperT : public TBase
{
    using Base = TBase;

    struct HasPrefixSuffixTag {};
    struct NoPrefixSuffixTag {};

public:
    typedef typename Base::SerialisedSeq SerialisedSeq;
    typedef typename Base::BasePtr BasePtr;

    virtual ~FieldWrapperT() noexcept = default;

protected:

    using Field = TField;

    explicit FieldWrapperT(Field& fieldRef)
      : m_field(fieldRef)
    {
    }

    virtual std::size_t lengthImpl() const override
    {
        return m_field.length();
    }

    virtual bool validImpl() const override
    {
        return m_field.valid();
    }

    Field& field()
    {
        return m_field;
    }

    const Field& field() const
    {
        return m_field;
    }

    virtual SerialisedSeq getSerialisedValueImpl() const override
    {
        SerialisedSeq seq;
        seq.reserve(m_field.length());
        auto iter = std::back_inserter(seq);
        [[maybe_unused]] auto es = m_field.write(iter, seq.max_size());
        assert(es == comms::ErrorStatus::Success);
        assert(seq.size() == m_field.length());
        return seq;
    }

    virtual bool setSerialisedValueImpl(const SerialisedSeq& value) override
    {
        if (value.empty()) {
            return false;
        }

        if ((!CollectionPrefixDetect<Field, IsCollection>::HasSizeFieldPrefix) &&
            (!CollectionPrefixDetect<Field, IsCollection>::HasSerLengthFieldPrefix) &&
            (!CollectionPrefixDetect<Field, IsCollection>::HasTrailingFieldSuffix) &&
            (!CollectionPrefixDetect<Field, IsCollection>::HasTerminationFieldSuffix)) {
            auto iter = &value[0];
            auto es = m_field.read(iter, value.size());
            return es == comms::ErrorStatus::Success;
        }

        SerialisedSeq newVal;
        if (!writeSerialisedSize(newVal, value.size(), SerialisedSizePrefixTag())) {
            return false;
        }

        if (!writeSerialisedLength(newVal, value.size(), SerialisedLengthPrefixTag())) {
            return false;
        }

        auto writeIter = std::back_inserter(newVal);
        std::copy(value.begin(), value.end(), writeIter);

        if (!writeTrailSuffix(newVal, SerialisedTrailSuffixTag())) {
            return false;
        }

        if (!writeTermSuffix(newVal, SerialisedTermSuffixTag())) {
            return false;
        }

        auto iter = &newVal[0];
        auto es = m_field.read(iter, newVal.size());
        return es == comms::ErrorStatus::Success;
    }

    virtual BasePtr upCloneImpl() override
    {
        return static_cast<Base*>(this)->clone();
    }

    virtual bool canWriteImpl() const override
    {
        return m_field.canWrite();
    }

    virtual void resetImpl() override
    {
        m_field = Field();
    }

private:
    template <typename T, bool TIsCollection>
    struct CollectionPrefixDetect
    {
        static const bool HasSizeFieldPrefix = false;
        static const bool HasSerLengthFieldPrefix = false;
        static const bool HasTrailingFieldSuffix = false;
        static const bool HasTerminationFieldSuffix = false;
    };

    template <typename T>
    struct CollectionPrefixDetect<T, true>
    {
        static const bool HasSizeFieldPrefix = Field::hasSizeFieldPrefix();
        static const bool HasSerLengthFieldPrefix = Field::hasSerLengthFieldPrefix();
        static const bool HasTrailingFieldSuffix = TField::hasTrailingFieldSuffix();
        static const bool HasTerminationFieldSuffix = Field::hasTerminationFieldSuffix();
    };

    static constexpr bool IsCollection = comms::field::isString<Field>() || comms::field::isArrayList<Field>();

    typedef typename std::conditional<
        CollectionPrefixDetect<Field, IsCollection>::HasSizeFieldPrefix,
        HasPrefixSuffixTag,
        NoPrefixSuffixTag
    >::type SerialisedSizePrefixTag;

    typedef typename std::conditional<
        CollectionPrefixDetect<Field, IsCollection>::HasSerLengthFieldPrefix,
        HasPrefixSuffixTag,
        NoPrefixSuffixTag
    >::type SerialisedLengthPrefixTag;

    typedef typename std::conditional<
        CollectionPrefixDetect<Field, IsCollection>::HasTrailingFieldSuffix,
        HasPrefixSuffixTag,
        NoPrefixSuffixTag
    >::type SerialisedTrailSuffixTag;

    typedef typename std::conditional<
        CollectionPrefixDetect<Field, IsCollection>::HasTerminationFieldSuffix,
        HasPrefixSuffixTag,
        NoPrefixSuffixTag
    >::type SerialisedTermSuffixTag;

    bool writeSerialisedSize(SerialisedSeq& seq, std::size_t sizeVal, HasPrefixSuffixTag)
    {
        typedef typename Field::SizeFieldPrefix SizePrefixField;

        SizePrefixField sizePrefixField;
        sizePrefixField.setValue(sizeVal);
        auto writeIter = std::back_inserter(seq);
        auto es = sizePrefixField.write(writeIter, seq.max_size() - seq.size());
        return es == comms::ErrorStatus::Success;
    }

    bool writeSerialisedSize([[maybe_unused]] SerialisedSeq& seq, [[maybe_unused]] std::size_t sizeVal, NoPrefixSuffixTag)
    {
        return true;
    }

    bool writeSerialisedLength(SerialisedSeq& seq, std::size_t sizeVal, HasPrefixSuffixTag)
    {
        typedef typename Field::SerLengthFieldPrefix LengthPrefixField;

        LengthPrefixField lengthPrefixField;
        lengthPrefixField.setValue(sizeVal);
        auto writeIter = std::back_inserter(seq);
        auto es = lengthPrefixField.write(writeIter, seq.max_size() - seq.size());
        return es == comms::ErrorStatus::Success;
    }

    bool writeSerialisedLength([[maybe_unused]] SerialisedSeq& seq, [[maybe_unused]] std::size_t sizeVal, NoPrefixSuffixTag)
    {
        return true;
    }

    bool writeTrailSuffix(SerialisedSeq& seq, HasPrefixSuffixTag)
    {
        typedef typename Field::TrailingFieldSuffix TrailingSuffixField;
        TrailingSuffixField trailingSuffixField;
        auto writeIter = std::back_inserter(seq);
        auto es = trailingSuffixField.write(writeIter, seq.max_size() - seq.size());
        return es == comms::ErrorStatus::Success;
    }

    bool writeTrailSuffix([[maybe_unused]] SerialisedSeq& seq, NoPrefixSuffixTag)
    {
        return true;
    }

    bool writeTermSuffix(SerialisedSeq& seq, HasPrefixSuffixTag)
    {
        typedef typename Field::TerminationFieldSuffix TermSuffixField;
        TermSuffixField termSuffixField;
        auto writeIter = std::back_inserter(seq);
        auto es = termSuffixField.write(writeIter, seq.max_size() - seq.size());
        return es == comms::ErrorStatus::Success;
    }

    bool writeTermSuffix([[maybe_unused]] SerialisedSeq& seq, NoPrefixSuffixTag)
    {
        return true;
    }


    Field& m_field;
};

typedef FieldWrapper::BasePtr FieldWrapperPtr;

}  // namespace field_wrapper

}  // namespace cc_tools_qt

COMMS_MSVC_WARNING_POP
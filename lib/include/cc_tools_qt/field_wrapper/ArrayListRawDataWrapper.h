//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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
#include <cassert>
#include <memory>
#include <limits>

#include <QtCore/QString>

#include "comms/comms.h"

#include "cc_tools_qt/ToolsField.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

class CC_API ArrayListRawDataWrapper : public ToolsField
{
public:

    typedef std::unique_ptr<ArrayListRawDataWrapper> Ptr;

    ArrayListRawDataWrapper();
    virtual ~ArrayListRawDataWrapper() noexcept;

    QString getValue() const;

    void setValue(const QString& val);

    int maxSize() const;

    int minSize() const;

    Ptr clone();

    bool getForcedShowAll() const;
    void setForcedShowAll(bool val = true);
    bool isTruncated() const;

protected:
    virtual QString getValueImpl() const = 0;
    virtual void setValueImpl(const QString& val) = 0;
    virtual int maxSizeImpl() const = 0;
    virtual int minSizeImpl() const = 0;
    virtual Ptr cloneImpl() = 0;

    void dispatchImpl(FieldWrapperHandler& handler);

    static const std::size_t TruncateLength = 128;

private:
    bool m_forcedShowAll = false;    
};

template <typename TField>
class ArrayListRawDataWrapperT : public ToolsFieldT<ArrayListRawDataWrapper, TField>
{
    using Base = ToolsFieldT<ArrayListRawDataWrapper, TField>;
    using Field = TField;

public:
    using SerialisedSeq = typename Base::SerialisedSeq;
    typedef typename Base::Ptr Ptr;

    explicit ArrayListRawDataWrapperT(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ArrayListRawDataWrapperT(const ArrayListRawDataWrapperT&) = default;
    ArrayListRawDataWrapperT(ArrayListRawDataWrapperT&&) = default;
    virtual ~ArrayListRawDataWrapperT() noexcept = default;

    ArrayListRawDataWrapperT& operator=(const ArrayListRawDataWrapperT&) = delete;

protected:

    virtual QString getValueImpl() const override
    {
        QString retStr;
        auto& dataField = Base::field();
        auto& data = dataField.value();

        int maxLen = static_cast<int>(Base::length() * 2);
        if (Base::isTruncated()) {
            maxLen = static_cast<decltype(maxLen)>(Base::TruncateLength * 2);
        }

        for (auto byte : data) {
            if (maxLen <= retStr.size()) {
                break;
            }

            retStr.append(QString("%1").arg(static_cast<uint>(byte), 2, 16, QChar('0')));
        }
        return retStr;
    }

    virtual void setValueImpl(const QString& val) override
    {
        SerialisedSeq data;
        QString byteStr;

        auto addByteToData =
            [&data, &byteStr]() mutable
            {
                bool ok = false;
                auto intVal = byteStr.toInt(&ok, 16);
                if (!ok) {
                    return;
                }

                data.push_back(static_cast<typename SerialisedSeq::value_type>(intVal));
                byteStr.clear();
            };

        for (auto ch : val) {
            if (((ch < '0') || ('9' < ch)) &&
                ((ch.toLower() < 'a') || ('f' < ch.toLower()))) {
                continue;
            }

            byteStr.append(ch);

            if (2 <= byteStr.size()) {
                addByteToData();
            }
        }

        if (!byteStr.isEmpty()) {
            byteStr.append('0');
            addByteToData();
        }

        Base::setSerialisedValueImpl(data);
    }

    virtual SerialisedSeq getSerialisedValueImpl() const override
    {
        auto serValue = Base::getSerialisedValueImpl();
        if (Base::isTruncated()) {
            serValue.resize(Base::TruncateLength);
        }
        
        return serValue;
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

    virtual int minSizeImpl() const override
    {
        return minSizeInternal(SizeExistanceTag());
    }

    virtual Ptr cloneImpl() override
    {
        return Ptr(new ArrayListRawDataWrapperT<TField>(Base::field()));
    }

private:
    struct SizeFieldExistsTag {};
    struct SerLengthFieldExistsTag {};
    struct FixedSizeTag {};
    struct NoLimitsTag {};

    typedef typename std::conditional<
        Field::hasSizeFieldPrefix(),
        SizeFieldExistsTag,
        typename std::conditional<
            Field::hasSerLengthFieldPrefix(),
            SerLengthFieldExistsTag,
            typename std::conditional<
                Field::hasFixedSize(),
                FixedSizeTag,
                NoLimitsTag
            >::type
        >::type
    >::type SizeExistanceTag;

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
        typedef typename Field::SizeFieldPrefix PrefixField;
        return maxSizeByPrefix<PrefixField>();
    }

    static int maxSizeInternal(SerLengthFieldExistsTag)
    {
        typedef typename Field::SerLengthFieldPrefix PrefixField;
        return maxSizeByPrefix<PrefixField>();
    }

    static int maxSizeInternal(FixedSizeTag)
    {
        return static_cast<int>(Field::fixedSize());
    }

    int maxSizeInternal(NoLimitsTag) const
    {
        return
            static_cast<int>(
                std::min(
                    static_cast<std::size_t>(std::numeric_limits<int>::max()),
                    Base::field().value().max_size()));
    }

    static int minSizeInternal(SizeFieldExistsTag)
    {
        return 0;
    }

    static int minSizeInternal(SerLengthFieldExistsTag)
    {
        return 0;
    }

    static int minSizeInternal(FixedSizeTag)
    {
        return static_cast<int>(Field::fixedSize());
    }

    int minSizeInternal(NoLimitsTag) const
    {
        return 0;
    }
};

using ArrayListRawDataWrapperPtr = ArrayListRawDataWrapper::Ptr;

template <typename TField>
ArrayListRawDataWrapperPtr
makeArrayListRawDataWrapper(TField& field)
{
    return
        ArrayListRawDataWrapperPtr(
            new ArrayListRawDataWrapperT<TField>(field));
}

}  // namespace field_wrapper

}  // namespace cc_tools_qt




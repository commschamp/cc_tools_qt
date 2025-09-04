//
// Copyright 2016 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsMessage.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariantMap>


namespace cc_tools_qt
{

namespace property
{

namespace message
{

template <typename TValue>
class ToolsMsgPropBase
{
public:
    using ValueType = TValue;

    ToolsMsgPropBase(const char* propName)
      : m_propName(propName)
    {
    }

    template <typename U>
    void setTo(U&& val, QObject& obj) const
    {
        obj.setProperty(m_propName, QVariant::fromValue(std::forward<U>(val)));
        assert(obj.property(m_propName).template canConvert<ValueType>());
    }

    template <typename U>
    void setTo(U&& val, QVariantMap& map) const
    {
        QString name(m_propName);
        map.insert(name, QVariant::fromValue(std::forward<U>(val)));
        assert(map.value(name).template canConvert<ValueType>());
    }

    ValueType getFrom(const QObject& obj, const ValueType& defaultVal = ValueType()) const
    {
        auto var = obj.property(m_propName);
        if ((!var.isValid()) || (!var.template canConvert<ValueType>())) {
            return defaultVal;
        }

        return var.template value<ValueType>();
    }

    ValueType getFrom(const QVariantMap& map, const ValueType& defaultVal = ValueType()) const
    {
        QString name(m_propName);
        auto var = map.value(name);
        if ((!var.isValid()) || (!var.template canConvert<ValueType>())) {
            return defaultVal;
        }

        return var.template value<ValueType>();
    }

    void copyFromTo(const QObject& from, QObject& to) const
    {
        auto var = from.property(m_propName);
        if (var.isValid()) {
            to.setProperty(m_propName, std::move(var));
        }
    }

private:
    const char* m_propName = nullptr;
};

class CC_TOOLS_API ToolsMsgType : public ToolsMsgPropBase<unsigned>
{
    using Base = ToolsMsgPropBase<unsigned>;
public:
    using ValueType = ToolsMessage::Type;

    ToolsMsgType();

    template <typename TTo>
    void setTo(ValueType val, TTo&& to)
    {
        Base::setTo(static_cast<Base::ValueType>(val), std::forward<TTo>(to));
    }

    template <typename TFrom>
    ValueType getFrom(TFrom&& from)
    {
        return static_cast<ValueType>(Base::getFrom(std::forward<TFrom>(from)));
    }
};

class CC_TOOLS_API ToolsMsgIdx : public ToolsMsgPropBase<unsigned>
{
    using Base = ToolsMsgPropBase<unsigned>;
public:
    ToolsMsgIdx();
};

class CC_TOOLS_API ToolsMsgTimestamp : public ToolsMsgPropBase<unsigned long long>
{
    using Base = ToolsMsgPropBase<unsigned long long>;
public:
    ToolsMsgTimestamp();
};

class CC_TOOLS_API ToolsMsgProtocolName : public ToolsMsgPropBase<QString>
{
    using Base = ToolsMsgPropBase<QString>;
public:
    ToolsMsgProtocolName();
};

class CC_TOOLS_API ToolsMsgTransportMsg : public ToolsMsgPropBase<ToolsMessagePtr>
{
    using Base = ToolsMsgPropBase<ToolsMessagePtr>;
public:
    ToolsMsgTransportMsg();
};

class CC_TOOLS_API ToolsMsgRawDataMsg : public ToolsMsgPropBase<ToolsMessagePtr>
{
    using Base = ToolsMsgPropBase<ToolsMessagePtr>;
public:
    ToolsMsgRawDataMsg();
};

class CC_TOOLS_API ToolsMsgExtraInfoMsg : public ToolsMsgPropBase<ToolsMessagePtr>
{
    using Base = ToolsMsgPropBase<ToolsMessagePtr>;
public:
    ToolsMsgExtraInfoMsg();
};

class CC_TOOLS_API ToolsMsgExtraInfo : public ToolsMsgPropBase<QVariantMap>
{
    using Base = ToolsMsgPropBase<QVariantMap>;
public:
    ToolsMsgExtraInfo();
};

class CC_TOOLS_API ToolsMsgForceExtraInfoExistence : public ToolsMsgPropBase<bool>
{
    using Base = ToolsMsgPropBase<bool>;
public:
    ToolsMsgForceExtraInfoExistence();
};

class CC_TOOLS_API ToolsMsgDelay : public ToolsMsgPropBase<unsigned long long>
{
    using Base = ToolsMsgPropBase<unsigned long long>;
public:
    ToolsMsgDelay();
};

class CC_TOOLS_API ToolsMsgDelayUnits : public ToolsMsgPropBase<QString>
{
    using Base = ToolsMsgPropBase<QString>;
public:
    ToolsMsgDelayUnits();
};

class CC_TOOLS_API ToolsMsgRepeatDuration : public ToolsMsgPropBase<unsigned long long>
{
    using Base = ToolsMsgPropBase<unsigned long long>;
public:
    ToolsMsgRepeatDuration();
};

class CC_TOOLS_API ToolsMsgRepeatDurationUnits : public ToolsMsgPropBase<QString>
{
    using Base = ToolsMsgPropBase<QString>;
public:
    ToolsMsgRepeatDurationUnits();
};

class CC_TOOLS_API ToolsMsgRepeatCount : public ToolsMsgPropBase<unsigned>
{
    using Base = ToolsMsgPropBase<unsigned>;
public:
    ToolsMsgRepeatCount();
};

class CC_TOOLS_API ToolsMsgScrollPos : public ToolsMsgPropBase<int>
{
    using Base = ToolsMsgPropBase<int>;
public:
    ToolsMsgScrollPos();
};

class CC_TOOLS_API ToolsMsgComment : public ToolsMsgPropBase<QString>
{
    using Base = ToolsMsgPropBase<QString>;
public:
    ToolsMsgComment();
};

}  // namespace message

}  // namespace property

}  // namespace cc_tools_qt



//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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

#include <cassert>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariantMap>

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsMessage.h"

namespace cc_tools_qt
{

namespace property
{

namespace message
{

template <typename TValue>
class PropBase
{
public:
    using ValueType = TValue;

    PropBase(const QString& name, const QByteArray& propName)
      : m_name(name),
        m_propName(propName)
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
        map.insert(m_name, QVariant::fromValue(std::forward<U>(val)));
        assert(map.value(m_name).template canConvert<ValueType>());
    }

    ValueType getFrom(const QObject& obj, const ValueType& defaultVal = ValueType()) const
    {
        auto var = obj.property(m_propName.constData());
        if ((!var.isValid()) || (!var.template canConvert<ValueType>())) {
            return defaultVal;
        }

        return var.template value<ValueType>();
    }

    ValueType getFrom(const QVariantMap& map, const ValueType& defaultVal = ValueType()) const
    {
        auto var = map.value(m_name);
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
    const QString& m_name;
    const QByteArray& m_propName;
};

class CC_TOOLS_API Type : public PropBase<unsigned>
{
    using Base = PropBase<unsigned>;
public:
    using ValueType = ToolsMessage::Type;

    Type() : Base(Name, PropName) {}

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

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API MsgIdx : public PropBase<unsigned>
{
    using Base = PropBase<unsigned>;
public:
    MsgIdx() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};


class CC_TOOLS_API Timestamp : public PropBase<unsigned long long>
{
    using Base = PropBase<unsigned long long>;
public:
    Timestamp() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API ProtocolName : public PropBase<QString>
{
    using Base = PropBase<QString>;
public:
    ProtocolName() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API TransportMsg : public PropBase<ToolsMessagePtr>
{
    using Base = PropBase<ToolsMessagePtr>;
public:
    TransportMsg() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API RawDataMsg : public PropBase<ToolsMessagePtr>
{
    using Base = PropBase<ToolsMessagePtr>;
public:
    RawDataMsg() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API ExtraInfoMsg : public PropBase<ToolsMessagePtr>
{
    using Base = PropBase<ToolsMessagePtr>;
public:
    ExtraInfoMsg() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API ExtraInfo : public PropBase<QVariantMap>
{
    using Base = PropBase<QVariantMap>;
public:
    ExtraInfo() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API ForceExtraInfoExistence : public PropBase<bool>
{
    using Base = PropBase<bool>;
public:
    ForceExtraInfoExistence() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};


class CC_TOOLS_API Delay : public PropBase<unsigned long long>
{
    using Base = PropBase<unsigned long long>;
public:
    Delay() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API DelayUnits : public PropBase<QString>
{
    using Base = PropBase<QString>;
public:
    DelayUnits() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};


class CC_TOOLS_API RepeatDuration : public PropBase<unsigned long long>
{
    using Base = PropBase<unsigned long long>;
public:
    RepeatDuration() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API RepeatDurationUnits : public PropBase<QString>
{
    using Base = PropBase<QString>;
public:
    RepeatDurationUnits() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API RepeatCount : public PropBase<unsigned>
{
    using Base = PropBase<unsigned>;
public:
    RepeatCount() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API ScrollPos : public PropBase<int>
{
    using Base = PropBase<int>;
public:
    ScrollPos() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;
};

class CC_TOOLS_API Comment : public PropBase<QString>
{
    using Base = PropBase<QString>;
public:
    Comment() : Base(Name, PropName) {}

private:
    static const QString Name;
    static const QByteArray PropName;

};

}  // namespace message

}  // namespace property

}  // namespace cc_tools_qt



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

#include "cc_tools_qt/property/field.h"

namespace cc_tools_qt
{

namespace property
{

namespace field
{

namespace
{

const QString& dataKey()
{
    static const QString Str("cc.data");
    return Str;
}

const QString& uncheckableKey()
{
    static const QString Str("cc.uncheckable");
    return Str;
}

const QString& indexHiddenKey()
{
    static const QString Str("cc.index_hidden");
    return Str;
}

}  // namespace

Common::Common() = default;
Common::Common(const Common&) = default;
Common::Common(Common&&) = default;

Common::Common(const QVariantMap& props)
{
    getFrom(props);
}

Common::Common(const QVariant& props)
{
    if (props.isValid() && props.canConvert<QVariantMap>()) {
        getFrom(props.value<QVariantMap>());
    }
}

Common::~Common() noexcept = default;

Common& Common::operator=(const Common&) = default;
Common& Common::operator=(Common&&) = default;

void Common::setTo([[maybe_unused]] QVariantMap& props) const
{
}

void Common::getFrom([[maybe_unused]] const QVariantMap& props)
{
}

QVariantMap Common::asMap() const
{
    QVariantMap props;
    setTo(props);
    return props;
}

Optional::Optional() = default;
Optional::Optional(const Optional&) = default;
Optional::Optional(Optional&&) = default;
Optional::Optional(const QVariantMap& props)
  : Base(props)
{
    getFrom(props);
}

Optional::Optional(const QVariant& props)
  : Base(props)
{
    if (props.isValid() && props.canConvert<QVariantMap>()) {
        getFrom(props.value<QVariantMap>());
    }
}

Optional::~Optional() noexcept = default;

Optional& Optional::operator=(const Optional&) = default;
Optional& Optional::operator=(Optional&&) = default;

const QVariantMap& Optional::field() const
{
    return m_field;
}

Optional& Optional::field(QVariantMap&& fieldProps)
{
    m_field = std::move(fieldProps);
    return *this;
}

Optional& Optional::field(const QVariantMap& fieldProps)
{
    m_field = fieldProps;
    return *this;
}

bool Optional::isUncheckable() const
{
    return m_uncheckable;
}

Optional& Optional::uncheckable(bool value)
{
    m_uncheckable = value;
    return *this;
}

QVariantMap Optional::asMap() const
{
    QVariantMap props;
    Base::setTo(props);
    Base::setElemTo(m_field, dataKey(), props);
    Base::setElemTo(m_uncheckable, uncheckableKey(), props);
    return props;
}

void Optional::getFrom(const QVariantMap& props)
{
    m_field = getElemFrom<QVariantMap>(props, dataKey());
    m_uncheckable = getElemFrom<bool>(props, uncheckableKey());
}


Variant::Variant() = default;
Variant::Variant(const Variant&) = default;
Variant::Variant(Variant&&) = default;
Variant::Variant(const QVariantMap& props) : Base(props)
{
    getFrom(props);
};

Variant::Variant(const QVariant& props) : Base(props)
{
    if (props.isValid() && props.canConvert<QVariantMap>()) {
        getFrom(props.value<QVariantMap>());
    }
};

Variant::~Variant() noexcept = default;

Variant& Variant::operator=(const Variant&) = default;
Variant& Variant::operator=(Variant&&) = default;

const Variant::MembersList& Variant::members() const
{
    return m_members;
}

Variant& Variant::add(QVariantMap&& memberProps)
{
    m_members.append(std::move(memberProps));
    return *this;
}

Variant& Variant::add(const QVariantMap& memberProps)
{
    m_members.append(memberProps);
    return *this;
}

bool Variant::isIndexHidden() const
{
    return m_indexHidden;
}

Variant& Variant::setIndexHidden(bool hiddenVal)
{
    m_indexHidden = hiddenVal;
    return *this;
}

QVariantMap Variant::asMap() const
{
    QVariantMap props;
    Base::setTo(props);
    Base::setElemTo(m_members, dataKey(), props);
    Base::setElemTo(m_indexHidden, indexHiddenKey(), props);
    return props;
}

void Variant::getFrom(const QVariantMap& props)
{
    m_members = getElemFrom<MembersList>(props, dataKey());
    m_indexHidden = getElemFrom<bool>(props, indexHiddenKey());
}

}  // namespace field

}  // namespace property

}  // namespace cc_tools_qt



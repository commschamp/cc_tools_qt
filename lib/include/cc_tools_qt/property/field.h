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

#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QVariantList>
#include <QtCore/QList>
#include <QtCore/QPair>

#include "comms/comms.h"
#include "cc_tools_qt/Api.h"

namespace cc_tools_qt
{

namespace property
{

namespace field
{

/// @brief Common set of properties
/// @details Common base class for all other properties description classes
/// @headerfile cc_tools_qt/property/field.h
class CC_API Common
{
public:
    /// @brief Default constructor
    Common();

    /// @brief Copy constructor
    Common(const Common&);

    /// @brief Move constructor
    Common(Common&&);

    /// @brief Construct from QVariantMap
    explicit Common(const QVariantMap& props);

    /// @brief Construct from QVariant that contains QVariantMap.
    explicit Common(const QVariant& props);

    /// @brief Destructor
    ~Common() noexcept;

    /// @brief Copy assignment operator
    Common& operator=(const Common&);

    /// @brief Move assignment operator
    Common& operator=(Common&&);

    /// @brief Copy all the properties value into provided properties map
    void setTo(QVariantMap& props) const;

    /// @brief Read the properties values from the provided map
    void getFrom(const QVariantMap& props);

    QVariantMap asMap() const;    

protected:

    /// @brief set element value to the map
    /// @param[in] val Value to set
    /// @param[in] name Name of the property
    /// @param[in, out] props Map to update
    template <typename U>
    static void setElemTo(U&& val, const QString& name, QVariantMap& props)
    {
        props.insert(name, QVariant::fromValue(std::forward<U>(val)));
    }

    /// @brief read element value from the map
    /// @param[in] props Properties map
    /// @param[in] name Property name
    /// @param[in] defaultVal Value to return if not found in map
    /// @return Value of found property in the map
    template <typename TValueType>
    static TValueType getElemFrom(
        const QVariantMap& props,
        const QString& name,
        const TValueType& defaultVal = TValueType())
    {
        auto var = props.value(name);
        if ((!var.isValid()) || (!var.canConvert<TValueType>())) {
            return defaultVal;
        }

        return var.value<TValueType>();
    }

};

namespace details
{

template <typename TField>
struct ForTag;

template <>
struct ForTag<comms::field::tag::Int>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Enum>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Bitmask>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Bitfield>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Bundle>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::String>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::RawArrayList>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::ArrayList>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Optional>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Float>
{
    using Type = cc_tools_qt::property::field::Common;
};

template <>
struct ForTag<comms::field::tag::Variant>
{
    using Type = cc_tools_qt::property::field::Common;
};

}  // namespace details

/// @brief Get proper properties management class for field.
/// @tparam TField Type of the field
/// @headerfile cc_tools_qt/property/field.h
template <typename TField>
using ForField = typename details::ForTag<typename TField::CommsTag>::Type;

}  // namespace field

}  // namespace property

}  // namespace cc_tools_qt


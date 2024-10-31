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

/// @brief Intermediate helper class to define properties describing one
/// @tparam TDerived Actual derived class
/// @headerfile cc_tools_qt/property/field.h
template <typename TDerived>
class CommonBase : public Common
{
    using Base = Common;
public:
    /// @brief Default constructor
    CommonBase() = default;

    /// @brief Copy constructor
    CommonBase(const CommonBase&) = default;

    /// @brief Move constructor
    CommonBase(CommonBase&&) = default;

    /// @brief Construct from QVariantMap
    explicit CommonBase(const QVariantMap& props) : Base(props) {}

    /// @brief Construct from QVariant that contains QVariantMap.
    explicit CommonBase(const QVariant& props) : Base(props) {}

    /// @brief Destructor
    ~CommonBase() noexcept = default;

    /// @brief Copy assignment
    CommonBase& operator=(const CommonBase&) = default;

    /// @brief Move assignment
    CommonBase& operator=(CommonBase&&) = default;
};

/// @brief Class to contain all the properties relevant to
///     @b comms::field::Variant fields.
/// @headerfile cc_tools_qt/property/field.h
class CC_API Variant : public CommonBase<Variant>
{
    typedef CommonBase<Variant> Base;
public:

    /// @brief List of properties of the contained fields.
    using MembersList = QList<QVariantMap>;

    /// @brief Default constructor
    Variant();

    /// @brief Copy constructor
    Variant(const Variant&);

    /// @brief Move constructor
    Variant(Variant&&);

    /// @brief Construct from QVariantMap
    Variant(const QVariantMap& props);

    /// @brief Construct from QVariant containing QVariantMap
    Variant(const QVariant& props);

    /// @brief Destructor
    ~Variant() noexcept;

    /// @brief Copy assignment
    Variant& operator=(const Variant&);

    /// @brief Move assignment
    Variant& operator=(Variant&&);

    /// @brief Get access to the contained fields' properties.
    const MembersList& members() const;

    /// @brief Add properties of the next member
    Variant& add(QVariantMap&& memberProps);

    /// @brief Add properties of the next member.
    Variant& add(const QVariantMap& memberProps);

    /// @brief Check the member index should be hidden when displaying field.
    bool isIndexHidden() const;

    /// @brief Set the member index should be hidden when displaying field.
    Variant& setIndexHidden(bool hiddenVal = true);

    /// @brief Retrieve all properties as map.
    QVariantMap asMap() const;
private:
    void getFrom(const QVariantMap& props);

    MembersList m_members;
    bool m_indexHidden = false;
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
    typedef cc_tools_qt::property::field::Variant Type;
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


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

#include <cassert>

#include <QtCore/QString>

#include "comms/comms.h"
#include "ProtocolMessageBase.h"
#include "cc_tools_qt.h"

namespace cc_tools_qt
{

namespace details
{

template <typename TMsgBase, typename TFields>
class TransportMessageImpl : public
    comms::MessageBase<
        TMsgBase,
        comms::option::NoIdImpl,
        comms::option::FieldsImpl<TFields>,
        comms::option::MsgType<TransportMessageImpl<TMsgBase, TFields> >
    >
{

};

}  // namespace details

/// @brief Base class for @b TransportMessage definition in @b protocol
///     plugin.
/// @tparam TMessage Common interface class for @b protocol plugin.
/// @tparam TAllFields All transport fields difined by <b>protocol stack</b>.
/// @headerfile cc_tools_qt/TransportMessageBase.h
template <typename TMessage, typename TAllFields>
class TransportMessageBase : public
    ProtocolMessageBase<
        details::TransportMessageImpl<TMessage, TAllFields>,
        TransportMessageBase<TMessage, TAllFields>
    >
{
public:
    /// @brief Destructor
    virtual ~TransportMessageBase() noexcept = default;

protected:
    /// @brief Overriding virtual cc_tools_qt::Message::nameImpl()
    virtual const char* nameImpl() const override
    {
        static const char* Str = "Generic Transport Message";
        return Str;
    }

    /// @brief Overriding virtual cc_tools_qt::Message::extraTransportFieldsPropertiesImpl()
    virtual const QVariantList&  extraTransportFieldsPropertiesImpl() const override
    {
        static const QVariantList List;
        return List;
    }

    /// @brief Overriding virtual cc_tools_qt::Message::idAsStringImpl()
    virtual QString idAsStringImpl() const override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return QString();
    }

    /// @brief Overriding virtual cc_tools_qt::Message::resetImpl()
    virtual void resetImpl() override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }

    /// @brief Overriding virtual cc_tools_qt::Message::assignImpl()
    virtual bool assignImpl([[maybe_unused]] const cc_tools_qt::Message& other) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return false;
    }
};

}  // namespace cc_tools_qt



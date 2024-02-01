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
#include <tuple>

#include <QtCore/QString>

#include "comms/comms.h"
#include "property/message.h"
#include "ProtocolMessageBase.h"
#include "cc_tools_qt.h"

namespace cc_tools_qt
{

namespace details
{

template <typename TMsgBase>
class InvalidMessageImpl : public
    comms::MessageBase<
        TMsgBase,
        comms::option::NoIdImpl,
        comms::option::FieldsImpl<std::tuple<> >,
        comms::option::MsgType<InvalidMessageImpl<TMsgBase> >
    >
{

};

}  // namespace details

template <typename TMessage>
class InvalidMessage : public
    ProtocolMessageBase<
        details::InvalidMessageImpl<TMessage>,
        InvalidMessage<TMessage>
    >
{
public:
    virtual ~InvalidMessage() noexcept = default;

protected:
    virtual const char*
    nameImpl() const override
    {
        if (property::message::TransportMsg().getFrom(*this)) {
            static const char* InvalidMsgStr = "???";
            return InvalidMsgStr;
        }

        static const char* GarbageStr = "-#-";
        return GarbageStr;
    }

    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        static const QVariantList Props;
        return Props;
    }

    virtual QString idAsStringImpl() const override
    {
        return QString();
    }

    virtual bool isValidImpl() const override
    {
        return false;
    }

    virtual void resetImpl() override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }

    virtual bool assignImpl([[maybe_unused]] const cc_tools_qt::Message& other) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return false;
    }

};

}  // namespace cc_tools_qt



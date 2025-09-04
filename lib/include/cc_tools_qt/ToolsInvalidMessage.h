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

#include "cc_tools_qt/ToolsMessageBase.h"
#include "cc_tools_qt/ToolsProtMsgInterface.h"
#include "cc_tools_qt/property/message.h"
#include "cc_tools_qt/version.h"

#include <QtCore/QString>

#include <cassert>
#include <tuple>

namespace cc_tools_qt
{

namespace details
{

template <typename TMsgBase, typename... TOpts>
class ToolInvalidMessageImpl : public
    comms::MessageBase<
        TMsgBase,
        comms::option::def::NoIdImpl,
        comms::option::def::ZeroFieldsImpl,
        comms::option::def::MsgType<ToolInvalidMessageImpl<TMsgBase, TOpts...>>
    >
{
};

}  // namespace details

template<typename TBase>
class ToolsInvalidMessage : public
    cc_tools_qt::ToolsMessageBase<
        TBase,
        details::ToolInvalidMessageImpl,
        ToolsInvalidMessage<TBase>
    >
{
    using Base =
        cc_tools_qt::ToolsMessageBase<
            TBase,
            details::ToolInvalidMessageImpl,
            ToolsInvalidMessage<TBase>
        >;
public:
    using DataSeq = typename Base::DataSeq;
    using FieldsList = typename Base::FieldsList;
    virtual ~ToolsInvalidMessage() noexcept = default;

protected:
    virtual const char* nameImpl() const override
    {
        if (property::message::ToolsMsgTransportMsg().getFrom(*this)) {
            static const char* InvalidMsgStr = "???";
            return InvalidMsgStr;
        }

        static const char* GarbageStr = "-#-";
        return GarbageStr;
    }

    virtual bool isValidImpl() const override
    {
        return false;
    }

    virtual qlonglong numericIdImpl() const override
    {
        return 0;
    }

    virtual void resetImpl() override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);
    }

    virtual bool assignImpl([[maybe_unused]] const cc_tools_qt::ToolsMessage& other) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);
        return false;
    }

    virtual DataSeq encodeDataImpl() const override
    {
        ToolsMessagePtr rawDataMsg = property::message::ToolsMsgRawDataMsg().getFrom(*this);
        if (!rawDataMsg) {
            assert(false);
            return DataSeq();
        }

        return rawDataMsg->encodeData();
    }

    virtual DataSeq encodeFramedImpl([[maybe_unused]] ToolsFrame& frame) const override
    {
        return encodeDataImpl();
    }

    virtual typename Base::Ptr cloneImpl() const override
    {
        ToolsMessagePtr rawDataMsg = property::message::ToolsMsgRawDataMsg().getFrom(*this);
        auto ptr = Base::cloneImpl();
        if (ptr && rawDataMsg) {
            ToolsMessagePtr p = rawDataMsg->clone();
            property::message::ToolsMsgRawDataMsg().setTo(std::move(p), *ptr);
        }

        return ptr;
    }

    virtual FieldsList transportFieldsImpl() override
    {
        return FieldsList();
    }
};

}  // namespace cc_tools_qt



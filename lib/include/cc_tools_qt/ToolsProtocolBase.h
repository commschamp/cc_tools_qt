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

#include "cc_tools_qt/ToolsExtraInfoMessage.h"
#include "cc_tools_qt/Protocol.h"
#include "cc_tools_qt/ToolsInvalidMessage.h"
#include "cc_tools_qt/ToolsRawDataMessage.h"

#include <algorithm>
#include <iterator>
#include <cassert>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QByteArray>

#include "cc_tools_qt/ErrorStatus.h"
#include "comms/util/ScopeGuard.h"
#include "comms/util/Tuple.h"


#include "Message.h"



namespace cc_tools_qt
{

/// @brief Helper class to define custom @ref Protocol
/// @details Provides the default implementation to most of the virtual
///     functions defined by @ref Protocol class.
/// @headerfile cc_tools_qt/ProtocolBase.h
template <typename TFrame>
class ToolsProtocolBase : public Protocol
{
protected:
    /// @brief Default constructor
    ToolsProtocolBase() = default;

    /// @brief Overriding implementation to Protocol::readImpl().
    virtual MessagesList readImpl(const DataInfo& dataInfo, bool final) override
    {
        auto allMsgs = m_frame.readData(dataInfo, final);
        for (auto& m : allMsgs) {
            setNameToMessageProperties(*m);
        }
        return allMsgs;
    }

    /// @brief Overriding implementation to Protocol::writeImpl().
    virtual DataInfoPtr writeImpl(Message& msg) override
    {
        auto dataInfo = makeDataInfo();
        assert(dataInfo);

        dataInfo->m_timestamp = DataInfo::TimestampClock::now();
        dataInfo->m_data = msg.encodeFramed(m_frame);
        dataInfo->m_extraProperties = getExtraInfoFromMessageProperties(msg);
        return dataInfo;
    }

    /// @brief Overriding implementation to Protocol::updateMessageImpl().
    virtual UpdateStatus updateMessageImpl(Message& msg) override
    {
        bool refreshed = msg.refreshMsg();
        m_frame.updateMessage(msg);
        if (refreshed) {
            return UpdateStatus::Changed;
        }

        return UpdateStatus::NoChange;        
    }

    /// @brief Overriding implementation to Protocol::createInvalidMessageImpl().
    virtual MessagePtr createInvalidMessageImpl() override
    {
        auto msg = m_frame.createInvalidMessage();
        setNameToMessageProperties(*msg);
        return msg;
    }

    /// @brief Overriding implementation to Protocol::createRawDataMessageImpl().
    virtual MessagePtr createRawDataMessageImpl() override
    {
        return m_frame.createRawDataMessage();
    }

    /// @brief Overriding implementation to Protocol::createExtraInfoMessageImpl().
    virtual MessagePtr createExtraInfoMessageImpl() override
    {
        return m_frame.createExtraInfoMessage();
    }

    /// @brief Overriding implementation to Protocol::createAllMessagesImpl().
    virtual MessagesList createAllMessagesImpl() override
    {
        return m_frame.createAllMessages();
    }

    /// @brief Overriding implementation to Protocol::createMessageImpl().
    virtual MessagePtr createMessageImpl(const QString& idAsString, unsigned idx) override
    {
        return m_frame.createMessage(idAsString, idx);
    }

private:
    TFrame m_frame;
};

}  // namespace cc_tools_qt



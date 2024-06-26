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

#include "cc_tools_qt/Protocol.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QByteArray>

#include "cc_tools_qt/property/message.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace cc_tools_qt
{

namespace 
{

const std::string& debugPrefix()
{
    static const std::string Str("(protocol)");
    return Str;
} 

std::string dataToStr(const DataInfo::DataSeq& data)
{
    std::stringstream stream;
    stream << std::hex;

    for (auto byte : data) {
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte) << ' ';
    }

    return stream.str();
}

} // namespace 
    

Protocol::~Protocol() noexcept = default;

const QString& Protocol::name() const
{
    return nameImpl();
}

Protocol::MessagesList Protocol::read(
    const DataInfo& dataInfo,
    bool final)
{
    unsigned long long milliseconds = 0U;

    if (1U <= m_debugLevel) {
        auto timestamp = dataInfo.m_timestamp;
        if (timestamp == DataInfo::Timestamp()) {
            timestamp = DataInfo::TimestampClock::now();
        }

        auto sinceEpoch = timestamp.time_since_epoch();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] " << debugPrefix() << " <-- " << dataInfo.m_data.size() << " bytes";
        if (2U <= m_debugLevel) {
            std::cout << " | " << dataToStr(dataInfo.m_data);
        }
        std::cout << std::endl;
    }

    auto messages = readImpl(dataInfo, final);
    if (1U <= m_debugLevel) {
        for (auto& msgPtr : messages) {
            std::cout << '[' << milliseconds << "] " << msgPtr->name() << " <-- " << debugPrefix() << std::endl;
        }
    }

    return messages;
}

DataInfoPtr Protocol::write(Message& msg)
{
    unsigned long long milliseconds = property::message::Timestamp().getFrom(msg);;
    if (1U <= m_debugLevel) {
        if (milliseconds == 0) {
            auto timestamp = DataInfo::TimestampClock::now();
            auto sinceEpoch = timestamp.time_since_epoch();
            milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        }

        std::cout << '[' << milliseconds << "] " << msg.name() << " --> " << debugPrefix() << std::endl;
    }

    if (msg.idAsString().isEmpty()) {

        auto rawDataMsg = property::message::RawDataMsg().getFrom(msg);
        if (!rawDataMsg) {
            return DataInfoPtr();
        }

        auto dataInfoPtr = makeDataInfo();
        assert(dataInfoPtr);

        dataInfoPtr->m_timestamp = DataInfo::TimestampClock::now();
        dataInfoPtr->m_data = rawDataMsg->encodeData();

        return dataInfoPtr;
    }

    auto dataPtr = writeImpl(msg);
    if (1U <= m_debugLevel) {
        std::cout << '[' << milliseconds << "] " << debugPrefix() << " --> " << dataPtr->m_data.size() << " bytes";
        if (2U <= m_debugLevel) {
            std::cout << " | " << dataToStr(dataPtr->m_data);
        }
        std::cout << std::endl;
    }    
    return dataPtr;
}

Protocol::MessagesList Protocol::createAllMessages()
{
    auto allMsgs = createAllMessagesImpl();
    QString prevId;
    unsigned prevIdx = 0U;
    for (auto& msgPtr : allMsgs) {
        unsigned idx = 0U;
        if (prevId == msgPtr->idAsString()) {
            idx = prevIdx + 1U;
        }

        prevId = msgPtr->idAsString();
        prevIdx = idx;
        property::message::MsgIdx().setTo(idx, *msgPtr);

        setNameToMessageProperties(*msgPtr);
        setForceExtraInfoExistenceToMessageProperties(*msgPtr);
        updateMessage(*msgPtr);
    }    

    return allMsgs;    
}

MessagePtr Protocol::createMessage(const QString& idAsString, unsigned idx)
{
    auto msgPtr = createMessageImpl(idAsString, idx);
    if (msgPtr) {
        property::message::MsgIdx().setTo(idx, *msgPtr);
    }
    return msgPtr;
}

Protocol::UpdateStatus Protocol::updateMessage(Message& msg)
{
    if (!msg.idAsString().isEmpty()) {
        return updateMessageImpl(msg);
    }

    auto extraInfo = getExtraInfoFromMessageProperties(msg);
    if (extraInfo.isEmpty()) {
        if (!property::message::ExtraInfoMsg().getFrom(msg)) {
            return UpdateStatus::NoChange;
        }

        setExtraInfoMsgToMessageProperties(MessagePtr(), msg);
        return UpdateStatus::NoChange;
    }

    auto infoMsg = createExtraInfoMessageImpl();
    if (!infoMsg) {
        [[maybe_unused]] static constexpr bool Info_must_be_created = false;
        assert(Info_must_be_created);        
        return UpdateStatus::NoChange;
    }

    auto jsonObj = QJsonObject::fromVariantMap(extraInfo);
    QJsonDocument doc(jsonObj);
    auto jsonByteArray = doc.toJson();
    MsgDataSeq dataSeq;
    dataSeq.reserve(static_cast<std::size_t>(jsonByteArray.size()));
    std::copy_n(jsonByteArray.constData(), jsonByteArray.size(), std::back_inserter(dataSeq));
    if (!infoMsg->decodeData(dataSeq)) {
        setExtraInfoMsgToMessageProperties(MessagePtr(), msg);
        return UpdateStatus::NoChange;
    }

    setExtraInfoMsgToMessageProperties(std::move(infoMsg), msg);
    return UpdateStatus::NoChange;
}

MessagePtr Protocol::cloneMessage(const Message& msg)
{
    if (msg.idAsString().isEmpty()) {
        MessagePtr clonedMsg;

        auto rawDataMsg = property::message::RawDataMsg().getFrom(msg);
        if (rawDataMsg) {
            auto data = rawDataMsg->encodeData();
            clonedMsg = createInvalidMessage(data);
        }
        else {
            clonedMsg = createInvalidMessageImpl();
        }

        if (!clonedMsg) {
            return clonedMsg;
        }

        auto extraInfoMap = getExtraInfoFromMessageProperties(msg);
        if (!extraInfoMap.isEmpty()) {
            setExtraInfoToMessageProperties(extraInfoMap, *clonedMsg);
            updateMessage(*clonedMsg);
        }

        return clonedMsg;
    }

    auto clonedMsg = cloneMessageImpl(msg);
    if (clonedMsg) {
        setNameToMessageProperties(*clonedMsg);
        updateMessage(*clonedMsg);
        property::message::ExtraInfo().copyFromTo(msg, *clonedMsg);
    }
    return clonedMsg;
}

MessagePtr Protocol::createInvalidMessage(const MsgDataSeq& data)
{
    auto rawDataMsg = createRawDataMessageImpl();
    if (!rawDataMsg) {
        return MessagePtr();
    }

    if (!rawDataMsg->decodeData(data)) {
        return MessagePtr();
    }

    auto invalidMsg = createInvalidMessageImpl();
    if (!invalidMsg) {
        return invalidMsg;
    }

    setRawDataToMessageProperties(std::move(rawDataMsg), *invalidMsg);
    return invalidMsg;
}

void Protocol::socketConnectionReport(bool connected)
{
    socketConnectionReportImpl(connected);
}

void Protocol::messageReceivedReport(MessagePtr msg)
{
    messageReceivedReportImpl(std::move(msg));
}

void Protocol::messageSentReport(MessagePtr msg)
{
    messageSentReportImpl(std::move(msg));
}

void Protocol::applyInterPluginConfig(const QVariantMap& props)
{
    applyInterPluginConfigImpl(props);
}

void Protocol::setDebugOutputLevel(unsigned level)
{
    m_debugLevel = level;
}

void Protocol::socketConnectionReportImpl([[maybe_unused]] bool connected)
{
}

void Protocol::messageReceivedReportImpl([[maybe_unused]] MessagePtr msg)
{
}

void Protocol::messageSentReportImpl([[maybe_unused]] MessagePtr msg)
{
}

void Protocol::applyInterPluginConfigImpl([[maybe_unused]] const QVariantMap& props)
{
}

void Protocol::setNameToMessageProperties(Message& msg)
{
    property::message::ProtocolName().setTo(name(), msg);
}

void Protocol::reportError(const QString& str)
{
    if (m_errorReportCallback) {
        m_errorReportCallback(str);
    }
}

void Protocol::sendMessageRequest(MessagePtr msg)
{
    if (m_sendMessageRequestCallback) {
        m_sendMessageRequestCallback(std::move(msg));
    }
}

void Protocol::reportInterPluginConfig(const QVariantMap& props)
{
    if (m_interPluginConfigReportCallback) {
        m_interPluginConfigReportCallback(props);
    }
}

unsigned Protocol::getDebugOutputLevel() const
{
    return m_debugLevel;
}

void Protocol::setTransportToMessageProperties(MessagePtr transportMsg, Message& msg)
{
    property::message::TransportMsg().setTo(std::move(transportMsg), msg);
}

void Protocol::setRawDataToMessageProperties(MessagePtr rawDataMsg, Message& msg)
{
    property::message::RawDataMsg().setTo(std::move(rawDataMsg), msg);
}

void Protocol::setExtraInfoMsgToMessageProperties(MessagePtr extraInfoMsg, Message& msg)
{
    property::message::ExtraInfoMsg().setTo(std::move(extraInfoMsg), msg);
}

MessagePtr Protocol::getExtraInfoMsgToMessageProperties(const Message& msg)
{
    return property::message::ExtraInfoMsg().getFrom(msg);
}

QVariantMap Protocol::getExtraInfoFromMessageProperties(const Message& msg)
{
    return property::message::ExtraInfo().getFrom(msg);
}

void Protocol::setExtraInfoToMessageProperties(
    const QVariantMap& extraInfo,
    Message& msg)
{
    property::message::ExtraInfo().setTo(extraInfo, msg);
}

void Protocol::mergeExtraInfoToMessageProperties(
    const QVariantMap& extraInfo,
    Message& msg)
{
    auto map = getExtraInfoFromMessageProperties(msg);
    for (auto& key : extraInfo.keys()) {
        map.insert(key, extraInfo.value(key));
    }
    setExtraInfoToMessageProperties(map, msg);
}

void Protocol::setForceExtraInfoExistenceToMessageProperties(Message& msg)
{
    property::message::ForceExtraInfoExistence().setTo(true, msg);
}

bool Protocol::getForceExtraInfoExistenceFromMessageProperties(const Message& msg)
{
    return property::message::ForceExtraInfoExistence().getFrom(msg);
}


}  // namespace cc_tools_qt



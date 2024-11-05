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

#include "cc_tools_qt/ToolsProtocol.h"

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

std::string dataToStr(const ToolsDataInfo::DataSeq& data)
{
    std::stringstream stream;
    stream << std::hex;

    for (auto byte : data) {
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte) << ' ';
    }

    return stream.str();
}

} // namespace 
    

ToolsProtocol::~ToolsProtocol() noexcept = default;

const QString& ToolsProtocol::name() const
{
    return nameImpl();
}

ToolsProtocol::MessagesList ToolsProtocol::read(
    const ToolsDataInfo& dataInfo,
    bool final)
{
    unsigned long long milliseconds = 0U;

    if (1U <= m_debugLevel) {
        auto timestamp = dataInfo.m_timestamp;
        if (timestamp == ToolsDataInfo::Timestamp()) {
            timestamp = ToolsDataInfo::TimestampClock::now();
        }

        auto sinceEpoch = timestamp.time_since_epoch();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] " << debugPrefix() << " <-- " << dataInfo.m_data.size() << " bytes";
        if (2U <= m_debugLevel) {
            std::cout << " | " << dataToStr(dataInfo.m_data);
        }
        std::cout << std::endl;
    }

    assert(m_frame);
    auto messages = m_frame->readData(dataInfo, final);
    for (auto& m : messages) {
        setNameToMessageProperties(*m);
    }

    if (1U <= m_debugLevel) {
        for (auto& msgPtr : messages) {
            std::cout << '[' << milliseconds << "] " << msgPtr->name() << " <-- " << debugPrefix() << std::endl;
        }
    }

    return messages;
}

ToolsDataInfoPtr ToolsProtocol::write(ToolsMessage& msg)
{
    unsigned long long milliseconds = property::message::Timestamp().getFrom(msg);;
    if (1U <= m_debugLevel) {
        if (milliseconds == 0) {
            auto timestamp = ToolsDataInfo::TimestampClock::now();
            auto sinceEpoch = timestamp.time_since_epoch();
            milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        }

        std::cout << '[' << milliseconds << "] " << msg.name() << " --> " << debugPrefix() << std::endl;
    }

    if (msg.idAsString().isEmpty()) {

        auto rawDataMsg = property::message::RawDataMsg().getFrom(msg);
        if (!rawDataMsg) {
            return ToolsDataInfoPtr();
        }

        auto dataInfoPtr = makeDataInfo();
        assert(dataInfoPtr);

        dataInfoPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();
        dataInfoPtr->m_data = rawDataMsg->encodeData();

        return dataInfoPtr;
    }

    auto dataInfo = makeDataInfo();
    assert(dataInfo);

    dataInfo->m_timestamp = ToolsDataInfo::TimestampClock::now();
    dataInfo->m_data = msg.encodeFramed(*m_frame);
    dataInfo->m_extraProperties = getExtraInfoFromMessageProperties(msg);
    if (1U <= m_debugLevel) {
        std::cout << '[' << milliseconds << "] " << debugPrefix() << " --> " << dataInfo->m_data.size() << " bytes";
        if (2U <= m_debugLevel) {
            std::cout << " | " << dataToStr(dataInfo->m_data);
        }
        std::cout << std::endl;
    } 

    return dataInfo;    
}

ToolsProtocol::MessagesList ToolsProtocol::createAllMessages()
{
    assert(m_frame);
    auto allMsgs = m_frame->createAllMessages();
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

ToolsMessagePtr ToolsProtocol::createMessage(const QString& idAsString, unsigned idx)
{
    assert(m_frame);
    auto msgPtr = m_frame->createMessage(idAsString, idx);
    if (msgPtr) {
        property::message::MsgIdx().setTo(idx, *msgPtr);
    }
    return msgPtr;
}

ToolsProtocol::UpdateStatus ToolsProtocol::updateMessage(ToolsMessage& msg)
{
    if (!msg.idAsString().isEmpty()) {
        bool refreshed = msg.refreshMsg();
        assert(m_frame);
        m_frame->updateMessage(msg);
        if (refreshed) {
            return UpdateStatus::Changed;
        }

        return UpdateStatus::NoChange;          
    }

    auto extraInfo = getExtraInfoFromMessageProperties(msg);
    if (extraInfo.isEmpty()) {
        if (!property::message::ExtraInfoMsg().getFrom(msg)) {
            return UpdateStatus::NoChange;
        }

        setExtraInfoMsgToMessageProperties(ToolsMessagePtr(), msg);
        return UpdateStatus::NoChange;
    }

    assert(m_frame);
    auto infoMsg = m_frame->createExtraInfoMessage();
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
        setExtraInfoMsgToMessageProperties(ToolsMessagePtr(), msg);
        return UpdateStatus::NoChange;
    }

    setExtraInfoMsgToMessageProperties(std::move(infoMsg), msg);
    return UpdateStatus::NoChange;
}

ToolsMessagePtr ToolsProtocol::cloneMessage(const ToolsMessage& msg)
{
    if (msg.idAsString().isEmpty()) {
        ToolsMessagePtr clonedMsg;

        auto rawDataMsg = property::message::RawDataMsg().getFrom(msg);
        if (rawDataMsg) {
            auto data = rawDataMsg->encodeData();
            clonedMsg = createInvalidMessage(data);
        }
        else {
            assert(m_frame);
            clonedMsg = m_frame->createInvalidMessage();
            setNameToMessageProperties(*clonedMsg);
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

    auto clonedMsg = msg.clone();
    if (clonedMsg) {
        setNameToMessageProperties(*clonedMsg);
        updateMessage(*clonedMsg);
        property::message::ExtraInfo().copyFromTo(msg, *clonedMsg);
    }
    return clonedMsg;
}

ToolsMessagePtr ToolsProtocol::createInvalidMessage(const MsgDataSeq& data)
{
    assert(m_frame);
    auto rawDataMsg = m_frame->createRawDataMessage();
    if (!rawDataMsg) {
        return ToolsMessagePtr();
    }

    if (!rawDataMsg->decodeData(data)) {
        return ToolsMessagePtr();
    }

    auto invalidMsg = m_frame->createInvalidMessage();
    if (!invalidMsg) {
        return invalidMsg;
    }

    setNameToMessageProperties(*invalidMsg);
    setRawDataToMessageProperties(std::move(rawDataMsg), *invalidMsg);
    return invalidMsg;
}

void ToolsProtocol::socketConnectionReport(bool connected)
{
    socketConnectionReportImpl(connected);
}

void ToolsProtocol::messageReceivedReport(ToolsMessagePtr msg)
{
    messageReceivedReportImpl(std::move(msg));
}

void ToolsProtocol::messageSentReport(ToolsMessagePtr msg)
{
    messageSentReportImpl(std::move(msg));
}

void ToolsProtocol::applyInterPluginConfig(const QVariantMap& props)
{
    applyInterPluginConfigImpl(props);
}

void ToolsProtocol::setDebugOutputLevel(unsigned level)
{
    m_debugLevel = level;
}

ToolsProtocol::ToolsProtocol(ToolsFramePtr frame) :
    m_frame(std::move(frame))
{
}

void ToolsProtocol::socketConnectionReportImpl([[maybe_unused]] bool connected)
{
}

void ToolsProtocol::messageReceivedReportImpl([[maybe_unused]] ToolsMessagePtr msg)
{
}

void ToolsProtocol::messageSentReportImpl([[maybe_unused]] ToolsMessagePtr msg)
{
}

void ToolsProtocol::applyInterPluginConfigImpl([[maybe_unused]] const QVariantMap& props)
{
}

void ToolsProtocol::setNameToMessageProperties(ToolsMessage& msg)
{
    property::message::ProtocolName().setTo(name(), msg);
}

void ToolsProtocol::reportError(const QString& str)
{
    if (m_errorReportCallback) {
        m_errorReportCallback(str);
    }
}

void ToolsProtocol::sendMessageRequest(ToolsMessagePtr msg)
{
    if (m_sendMessageRequestCallback) {
        m_sendMessageRequestCallback(std::move(msg));
    }
}

void ToolsProtocol::reportInterPluginConfig(const QVariantMap& props)
{
    if (m_interPluginConfigReportCallback) {
        m_interPluginConfigReportCallback(props);
    }
}

unsigned ToolsProtocol::getDebugOutputLevel() const
{
    return m_debugLevel;
}

void ToolsProtocol::setTransportToMessageProperties(ToolsMessagePtr transportMsg, ToolsMessage& msg)
{
    property::message::TransportMsg().setTo(std::move(transportMsg), msg);
}

void ToolsProtocol::setRawDataToMessageProperties(ToolsMessagePtr rawDataMsg, ToolsMessage& msg)
{
    property::message::RawDataMsg().setTo(std::move(rawDataMsg), msg);
}

void ToolsProtocol::setExtraInfoMsgToMessageProperties(ToolsMessagePtr extraInfoMsg, ToolsMessage& msg)
{
    property::message::ExtraInfoMsg().setTo(std::move(extraInfoMsg), msg);
}

ToolsMessagePtr ToolsProtocol::getExtraInfoMsgToMessageProperties(const ToolsMessage& msg)
{
    return property::message::ExtraInfoMsg().getFrom(msg);
}

QVariantMap ToolsProtocol::getExtraInfoFromMessageProperties(const ToolsMessage& msg)
{
    return property::message::ExtraInfo().getFrom(msg);
}

void ToolsProtocol::setExtraInfoToMessageProperties(
    const QVariantMap& extraInfo,
    ToolsMessage& msg)
{
    property::message::ExtraInfo().setTo(extraInfo, msg);
}

void ToolsProtocol::mergeExtraInfoToMessageProperties(
    const QVariantMap& extraInfo,
    ToolsMessage& msg)
{
    auto map = getExtraInfoFromMessageProperties(msg);
    for (auto& key : extraInfo.keys()) {
        map.insert(key, extraInfo.value(key));
    }
    setExtraInfoToMessageProperties(map, msg);
}

void ToolsProtocol::setForceExtraInfoExistenceToMessageProperties(ToolsMessage& msg)
{
    property::message::ForceExtraInfoExistence().setTo(true, msg);
}

bool ToolsProtocol::getForceExtraInfoExistenceFromMessageProperties(const ToolsMessage& msg)
{
    return property::message::ForceExtraInfoExistence().getFrom(msg);
}


}  // namespace cc_tools_qt



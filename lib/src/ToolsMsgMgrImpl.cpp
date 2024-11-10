//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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


#include "ToolsMsgMgrImpl.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>

#include <QtCore/QVariant>

#include "comms/util/ScopeGuard.h"
#include "cc_tools_qt/property/message.h"

namespace cc_tools_qt
{

namespace
{

class SeqNumber : public property::message::ToolsMsgPropBase<unsigned long long>
{
    using Base = property::message::ToolsMsgPropBase<unsigned long long>;
public:
    SeqNumber() : Base("cc.msg_num") {};
};

void updateMsgTimestamp(ToolsMessage& msg, const ToolsDataInfo::Timestamp& timestamp)
{
    auto sinceEpoch = timestamp.time_since_epoch();
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch);
    property::message::ToolsMsgTimestamp().setTo(milliseconds.count(), msg);
}

}  // namespace

ToolsMsgMgrImpl::ToolsMsgMgrImpl()
{
}

ToolsMsgMgrImpl::~ToolsMsgMgrImpl() noexcept = default;

void ToolsMsgMgrImpl::start()
{
    if (m_running) {
        [[maybe_unused]] static constexpr bool Already_running = false;
        assert(Already_running);         
        return;
    }

    if (m_socket) {
        m_socket->start();
    }

    for (auto& f : m_filters) {
        f->start();
    }

    m_running = true;
}

void ToolsMsgMgrImpl::stop()
{
    if (!m_running) {
        [[maybe_unused]] static constexpr bool Already_stopped = false;
        assert(Already_stopped);   
        return;
    }

    for (auto& f : m_filters) {
        f->stop();
    }

    if (m_socket) {
        m_socket->stop();
    }

    m_running = false;
}

void ToolsMsgMgrImpl::clear()
{
    if (m_running) {
        [[maybe_unused]] static constexpr bool Still_running = false;
        assert(Still_running); ;
        stop();
    }

    m_socket.reset();
    m_protocol.reset();
    m_filters.clear();
}

ToolsSocketPtr ToolsMsgMgrImpl::getSocket() const
{
    return m_socket;
}

ToolsProtocolPtr ToolsMsgMgrImpl::getProtocol() const
{
    return m_protocol;
}

void ToolsMsgMgrImpl::setRecvEnabled(bool enabled)
{
    m_recvEnabled = enabled;
}

void ToolsMsgMgrImpl::deleteMsgs(const MessagesList& msgs)
{
    auto listIter = msgs.begin();
    auto storedIter = m_allMsgs.begin();
    auto prevStoredIter = storedIter;
    while ((listIter != msgs.end()) && (storedIter != m_allMsgs.end())) {
        auto listMsgNum = SeqNumber().getFrom(**listIter);
        auto storedMsgNum = SeqNumber().getFrom(**storedIter);

        if (storedMsgNum == listMsgNum) {
            ++listIter;
            ++storedIter;
            continue;
        }

        m_allMsgs.erase(prevStoredIter, storedIter);
        ++storedIter;
        prevStoredIter = storedIter;
    }

    m_allMsgs.erase(prevStoredIter, storedIter);
}

void ToolsMsgMgrImpl::deleteMsg(ToolsMessagePtr msg)
{
    assert(!m_allMsgs.empty());
    assert(msg);

    auto msgNum = SeqNumber().getFrom(*msg);

    auto iter = std::find_if(
        m_allMsgs.begin(),
        m_allMsgs.end(),
        [msgNum](const ToolsMessagePtr& msgTmp) -> bool
        {
            return SeqNumber().getFrom(*msgTmp) == msgNum;
        });

    if (iter == m_allMsgs.end()) {
        [[maybe_unused]] static constexpr bool Deleting_non_existing_message = false;
        assert(Deleting_non_existing_message);         
        return;
    }

    m_allMsgs.erase(iter);
}

void ToolsMsgMgrImpl::sendMsgs(MessagesList&& msgs)
{
    if (msgs.empty() || (!m_socket) || (!m_protocol)) {
        return;
    }

    for (auto& msgPtr : msgs) {
        if (!msgPtr) {
            continue;
        }

        auto updateMsgGuard =
            comms::util::makeScopeGuard(
                [this, &msgPtr]()
                {
                    updateInternalId(*msgPtr);
                    property::message::ToolsMsgType().setTo(MsgType::Sent, *msgPtr);
                    auto now = ToolsDataInfo::TimestampClock::now();
                    updateMsgTimestamp(*msgPtr, now);
                    m_allMsgs.push_back(msgPtr);
                    reportMsgAdded(msgPtr);
                });

        auto dataInfoPtr = m_protocol->write(*msgPtr);
        if (!dataInfoPtr) {
            continue;
        }

        QList <ToolsDataInfoPtr> data;
        data.append(std::move(dataInfoPtr));
        for (auto iter = m_filters.rbegin(); iter != m_filters.rend(); ++iter) {
            auto& filter = *iter;
            if (data.isEmpty()) {
                break;
            }

            QList <ToolsDataInfoPtr> dataTmp;
            for (auto& d : data) {
                dataTmp.append(filter->sendData(d));
            }

            data.swap(dataTmp);
        }

        if (data.isEmpty()) {
            continue;
        }

        for (auto& d : data) {
            m_socket->sendData(d);

            if (!d->m_extraProperties.isEmpty()) {
                auto map = property::message::ToolsMsgExtraInfo().getFrom(*msgPtr);
                for (auto& key : d->m_extraProperties.keys()) {
                    map.insert(key, d->m_extraProperties.value(key));
                }
                property::message::ToolsMsgExtraInfo().setTo(std::move(map), *msgPtr);
                m_protocol->updateMessage(*msgPtr);
            }
        }

        m_protocol->messageSentReport(msgPtr);
    }
}

void ToolsMsgMgrImpl::addMsgs(const MessagesList& msgs, bool reportAdded)
{
    for (auto& m : msgs) {
        if (!m) {
            [[maybe_unused]] static constexpr bool Invalid_message_in_the_list = false;
            assert(Invalid_message_in_the_list);
            continue;
        }

        if (property::message::ToolsMsgType().getFrom(*m) == MsgType::Invalid) {
            [[maybe_unused]] static constexpr bool Invalid_type_of_message = false;
            assert(Invalid_type_of_message);            
            continue;
        }

        if (property::message::ToolsMsgTimestamp().getFrom(*m) == 0) {
            auto now = ToolsDataInfo::TimestampClock::now();
            updateMsgTimestamp(*m, now);
        }

        updateInternalId(*m);
        if (reportAdded) {
            reportMsgAdded(m);
        }
        m_allMsgs.push_back(m);
    }
}

void ToolsMsgMgrImpl::setSocket(ToolsSocketPtr socket)
{
    if (!socket) {
        m_socket.reset();
        return;
    }

    connect(
        socket.get(), &ToolsSocket::sigDataReceivedReport,
        this, &ToolsMsgMgrImpl::socketDataReceived
    );        

    connect(
        socket.get(), &ToolsSocket::sigErrorReport,
        this, &ToolsMsgMgrImpl::socketErrorReport
    );        

    connect(
        socket.get(), &ToolsSocket::sigConnectionStatusReport,
        this, &ToolsMsgMgrImpl::socketConnectionReport
    );

    m_socket = std::move(socket);
}

void ToolsMsgMgrImpl::setProtocol(ToolsProtocolPtr protocol)
{
    if (m_protocol) {
        m_protocol->setErrorReportCallback(nullptr);
        m_protocol->setSendMessageRequestCallback(nullptr);
    }

    assert(protocol);
    protocol->setErrorReportCallback(
        [this](const QString& str)
        {
            reportError(str);
        });

    protocol->setSendMessageRequestCallback(
        [this](ToolsMessagePtr msg)
        {
            MessagesList msgsList;
            msgsList.push_back(std::move(msg));
            sendMsgs(std::move(msgsList));
        });

    m_protocol = std::move(protocol);
}

void ToolsMsgMgrImpl::addFilter(ToolsFilterPtr filter)
{
    if (!filter) {
        return;
    }

    connect(
        filter.get(), &ToolsFilter::sigDataToSendReport,
        this, &ToolsMsgMgrImpl::filterDataToSendReport
    );

    connect(
        filter.get(), &ToolsFilter::sigErrorReport,
        this, &ToolsMsgMgrImpl::filterErrorReport
    );

    m_filters.push_back(std::move(filter));
}

void ToolsMsgMgrImpl::socketErrorReport(const QString& msg)
{
    if (m_socket.get() != sender()) {
        return;
    }

    reportError(msg);
}

void ToolsMsgMgrImpl::socketConnectionReport(bool connected)
{
    if (m_socket.get() != sender()) {
        return;
    }

    for (auto& filter : m_filters) {
        filter->socketConnectionReport(connected);
    }

    if (m_protocol) {
        m_protocol->socketConnectionReport(connected);
    }
    
    reportSocketConnectionStatus(connected);
}

void ToolsMsgMgrImpl::socketDataReceived(ToolsDataInfoPtr dataInfoPtr)
{
    if (m_socket.get() != sender()) {
        return;
    }

    if ((!m_recvEnabled) || !(m_protocol) || (!dataInfoPtr)) {
        return;
    }

    QList<ToolsDataInfoPtr> data;
    data.append(dataInfoPtr);
    for (auto filt : m_filters) {
        assert(filt);

        if (data.isEmpty()) {
            return;
        }

        QList<ToolsDataInfoPtr> dataTmp;
        for (auto& d : data) {
            dataTmp.append(filt->recvData(d));
        }

        data.swap(dataTmp);
    }

    if (data.isEmpty()) {
        return;
    }

    MessagesList msgsList;
    while (!data.isEmpty()) {
        auto nextDataPtr = data.front();
        data.pop_front();

        auto msgs = m_protocol->read(*nextDataPtr);
        msgsList.insert(msgsList.end(), msgs.begin(), msgs.end());
    }

    if (msgsList.empty()) {
        return;
    }

    for (auto& m : msgsList) {
        assert(m);
        updateInternalId(*m);
        property::message::ToolsMsgType().setTo(MsgType::Received, *m);

        static const ToolsDataInfo::Timestamp DefaultTimestamp;
        if (dataInfoPtr->m_timestamp != DefaultTimestamp) {
            updateMsgTimestamp(*m, dataInfoPtr->m_timestamp);
        }
        else {
            auto now = ToolsDataInfo::TimestampClock::now();
            updateMsgTimestamp(*m, now);
        }

        m_protocol->messageReceivedReport(m);
        reportMsgAdded(m);
    }

    m_allMsgs.splice(m_allMsgs.end(), std::move(msgsList));
}

void ToolsMsgMgrImpl::filterErrorReport(const QString& msg)
{
    auto iter = 
        std::find_if(
            m_filters.begin() ,m_filters.end(),
            [sndr = sender()](auto& f)
            {
                return f.get() == sndr;
            });

    if (iter == m_filters.end()) {
        return;
    }

    reportError(msg);
}

void ToolsMsgMgrImpl::filterDataToSendReport(ToolsDataInfoPtr dataInfoPtr)
{
    if (!dataInfoPtr) {
        return;
    }

    auto senderIter = 
        std::find_if(
            m_filters.begin() ,m_filters.end(),
            [sndr = sender()](auto& f)
            {
                return f.get() == sndr;
            });

    if (senderIter == m_filters.end()) {
        return;
    }

    auto filterIdx = static_cast<unsigned>(std::distance(m_filters.begin(), senderIter));

    assert(filterIdx < m_filters.size());
    auto revIdx = m_filters.size() - filterIdx;

    QList<ToolsDataInfoPtr> data;
    data.append(std::move(dataInfoPtr));
    for (auto iter = m_filters.rbegin() + static_cast<std::intmax_t>(revIdx); iter != m_filters.rend(); ++iter) {

        if (data.isEmpty()) {
            break;
        }

        auto nextFilter = *iter;

        QList<ToolsDataInfoPtr> dataTmp;
        for (auto& d : data) {
            dataTmp.append(nextFilter->sendData(d));
        }

        data.swap(dataTmp);
    }

    if (!m_socket) {
        return;
    }

    for (auto& d : data) {
        m_socket->sendData(std::move(d));
    }
}

void ToolsMsgMgrImpl::updateInternalId(ToolsMessage& msg)
{
    SeqNumber().setTo(m_nextMsgNum, msg);
    ++m_nextMsgNum;
    assert(0 < m_nextMsgNum); // wrap around is not supported
}

void ToolsMsgMgrImpl::reportMsgAdded(ToolsMessagePtr msg)
{
    if (m_msgAddedCallback) {
        m_msgAddedCallback(std::move(msg));
    }
}

void ToolsMsgMgrImpl::reportError(const QString& error)
{
    auto timestamp = std::chrono::high_resolution_clock::now();
    auto sinceEpoch = timestamp.time_since_epoch();
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
    std::cerr << '[' << milliseconds << "] ERROR: " << error.toStdString() << std::endl;
    
    if (m_errorReportCallback) {
        m_errorReportCallback(error);
    }
}

void ToolsMsgMgrImpl::reportSocketConnectionStatus(bool connected)
{
    if (m_socketConnectionStatusReportCallback) {
        m_socketConnectionStatusReportCallback(connected);
    }
}

}  // namespace cc_tools_qt


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

#include "ToolsMsgSendMgrImpl.h"

#include <cassert>

#include "cc_tools_qt/property/message.h"

namespace cc_tools_qt
{

ToolsMsgSendMgrImpl::ToolsMsgSendMgrImpl()
  : m_timer(this)
{
    connect(
        &m_timer, SIGNAL(timeout()),
        this, SLOT(sendPendingAndWait()));
}

ToolsMsgSendMgrImpl::~ToolsMsgSendMgrImpl() noexcept = default;

void ToolsMsgSendMgrImpl::start(ToolsProtocolPtr protocol, const MessagesList& msgs)
{
    [[maybe_unused]] static constexpr bool The_previous_sending_must_be_stopped_first = false;
    assert(m_msgsToSend.empty() || The_previous_sending_must_be_stopped_first);
    m_protocol = std::move(protocol);
    for (auto& m : msgs) {
        auto clonedMsg = m_protocol->cloneMessage(*m);
        property::message::ToolsMsgDelay().copyFromTo(*m, *clonedMsg);
        property::message::ToolsMsgDelayUnits().copyFromTo(*m, *clonedMsg);
        property::message::ToolsMsgRepeatDuration().copyFromTo(*m, *clonedMsg);
        property::message::ToolsMsgRepeatDurationUnits().copyFromTo(*m, *clonedMsg);
        property::message::ToolsMsgRepeatCount().copyFromTo(*m, *clonedMsg);
        property::message::ToolsMsgComment().copyFromTo(*m, *clonedMsg);
        auto extraProps = property::message::ToolsMsgExtraInfo().getFrom(*m);
        if (!extraProps.isEmpty()) {
            property::message::ToolsMsgExtraInfo().setTo(std::move(extraProps), *clonedMsg);
            m_protocol->updateMessage(*clonedMsg);
            assert(!property::message::ToolsMsgExtraInfo().getFrom(*clonedMsg).isEmpty());
        }

        m_msgsToSend.push_back(std::move(clonedMsg));
    }
    sendPendingAndWait();
}

void ToolsMsgSendMgrImpl::stop()
{
    m_timer.stop();
    m_protocol.reset();
    m_msgsToSend.clear();
}

void ToolsMsgSendMgrImpl::sendPendingAndWait()
{
    m_timer.stop();
    auto iter = m_msgsToSend.begin();
    for (; iter != m_msgsToSend.end(); ++iter) {
        auto& msg = *iter;
        assert(msg);
        auto delay = property::message::ToolsMsgDelay().getFrom(*msg);
        if (delay != 0U) {
            break;
        }
    }

    decltype(m_msgsToSend) nextMsgsToSend;
    nextMsgsToSend.splice(
        nextMsgsToSend.end(), m_msgsToSend, m_msgsToSend.begin(), iter);

    for (auto& msgToSend : nextMsgsToSend) {
        auto repeatMs = property::message::ToolsMsgRepeatDuration().getFrom(*msgToSend);
        auto repeatCount = property::message::ToolsMsgRepeatCount().getFrom(*msgToSend, 1U);

        bool reinsert =
            (0U < repeatMs) &&
            ((repeatCount == 0U) || (1U < repeatCount));

        if (reinsert) {
            if (!m_protocol) {
                [[maybe_unused]] static constexpr bool Protocol_must_be_valid = false;
                assert(Protocol_must_be_valid);                 
                continue;
            }

            auto newDelay = repeatMs;
            auto reinsertIter =
                std::find_if(
                    m_msgsToSend.begin(), m_msgsToSend.end(),
                    [&newDelay](ToolsMessagePtr mPtr) mutable -> bool
                    {
                        assert(mPtr);
                        auto mDelay = property::message::ToolsMsgDelay().getFrom(*mPtr);
                        if (newDelay < mDelay) {
                            return true;
                        }
                        newDelay -= mDelay;
                        return false;
                    });

            if (reinsertIter != m_msgsToSend.end()) {
                auto& msgToUpdate = *reinsertIter;
                assert(msgToUpdate);
                auto mDelay = property::message::ToolsMsgDelay().getFrom(*msgToUpdate);
                property::message::ToolsMsgDelay().setTo(mDelay - newDelay, *msgToUpdate);
            }

            auto clonedMsg = m_protocol->cloneMessage(*msgToSend);
            auto extraProps = property::message::ToolsMsgExtraInfo().getFrom(*msgToSend);
            if (!extraProps.isEmpty()) {
                property::message::ToolsMsgExtraInfo().setTo(std::move(extraProps), *clonedMsg);
                m_protocol->updateMessage(*clonedMsg);
            }

            std::swap(clonedMsg, msgToSend);
            property::message::ToolsMsgDelay().setTo(newDelay, *clonedMsg);

            if (repeatCount != 0) {
                property::message::ToolsMsgRepeatCount().setTo(repeatCount - 1, *clonedMsg);
            }

            m_msgsToSend.insert(reinsertIter, std::move(clonedMsg));
        }
    }

    if (!m_msgsToSend.empty()) {
        auto& msg = m_msgsToSend.front();
        assert(msg);
        auto delay = property::message::ToolsMsgDelay().getFrom(*msg);
        assert(0 < delay);
        property::message::ToolsMsgDelay().setTo(0, *msg);
        m_timer.setSingleShot(true);
        m_timer.start(static_cast<int>(delay));
    }

    if ((!nextMsgsToSend.empty()) && m_sendCallback) {
        m_sendCallback(std::move(nextMsgsToSend));
    }

    if (m_msgsToSend.empty() && m_sendCompleteCallback) {
        m_sendCompleteCallback();
    }
}
}  // namespace cc_tools_qt

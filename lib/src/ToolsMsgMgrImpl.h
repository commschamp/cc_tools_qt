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


#pragma once

#include <vector>

#include "cc_tools_qt/ToolsMsgMgr.h"

namespace cc_tools_qt
{

class ToolsMsgMgrImpl
{
public:
    using MessagesList = ToolsMsgMgr::MessagesList;
    using MsgType = ToolsMsgMgr::MsgType;

    ToolsMsgMgrImpl();
    ~ToolsMsgMgrImpl() noexcept;

    void start();
    void stop();
    void clear();

    ToolsSocketPtr getSocket() const;
    ToolsProtocolPtr getProtocol() const;
    void setRecvEnabled(bool enabled);

    void deleteMsg(ToolsMessagePtr msg);
    void deleteMsgs(const MessagesList& msgs);
    void deleteAllMsgs()
    {
        m_allMsgs.clear();
    }

    void sendMsgs(MessagesList&& msgs);

    const MessagesList& getAllMsgs() const
    {
        return m_allMsgs;
    }

    void addMsgs(const MessagesList& msgs, bool reportAdded);

    void setSocket(ToolsSocketPtr socket);
    void setProtocol(ToolsProtocolPtr protocol);
    void addFilter(ToolsFilterPtr filter);

    using MsgAddedCallbackFunc = ToolsMsgMgr::MsgAddedCallbackFunc;
    using ErrorReportCallbackFunc = ToolsMsgMgr::ErrorReportCallbackFunc;
    using SocketConnectionStatusReportCallbackFunc = ToolsMsgMgr::SocketConnectionStatusReportCallbackFunc;

    template <typename TFunc>
    void setMsgAddedCallbackFunc(TFunc&& func)
    {
        m_msgAddedCallback = std::forward<TFunc>(func);
    }

    template <typename TFunc>
    void setErrorReportCallbackFunc(TFunc&& func)
    {
        m_errorReportCallback = std::forward<TFunc>(func);
    }

    template <typename TFunc>
    void setSocketConnectionStatusReportCallbackFunc(TFunc&& func)
    {
        m_socketConnectionStatusReportCallback = std::forward<TFunc>(func);
    }

private:
    using MsgNumberType = unsigned long long;
    using FiltersList = std::vector<ToolsFilterPtr>;

    void socketDataReceived(ToolsDataInfoPtr dataInfoPtr);
    void updateInternalId(ToolsMessage& msg);
    void reportMsgAdded(ToolsMessagePtr msg);
    void reportError(const QString& error);
    void reportSocketConnectionStatus(bool connected);

    MessagesList m_allMsgs;
    bool m_recvEnabled = false;

    ToolsSocketPtr m_socket;
    ToolsProtocolPtr m_protocol;
    FiltersList m_filters;
    MsgNumberType m_nextMsgNum = 1;
    bool m_running = false;

    MsgAddedCallbackFunc m_msgAddedCallback;
    ErrorReportCallbackFunc m_errorReportCallback;
    SocketConnectionStatusReportCallbackFunc m_socketConnectionStatusReportCallback;
};

}  // namespace cc_tools_qt

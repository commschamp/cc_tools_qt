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

#include "cc_tools_qt/MsgMgr.h"

namespace cc_tools_qt
{

class MsgMgrImpl
{
public:
    typedef MsgMgr::AllMessages AllMessages;
    typedef MsgMgr::MessagesList MessagesList;

    typedef MsgMgr::MsgType MsgType;

    MsgMgrImpl();
    ~MsgMgrImpl() noexcept;

    void start();
    void stop();
    void clear();

    SocketPtr getSocket() const;
    ToolsProtocolPtr getProtocol() const;
    void setRecvEnabled(bool enabled);

    void deleteMsg(MessagePtr msg);
    void deleteMsgs(const MessagesList& msgs);
    void deleteAllMsgs()
    {
        m_allMsgs.clear();
    }

    void sendMsgs(MessagesList&& msgs);

    const AllMessages& getAllMsgs() const
    {
        return m_allMsgs;
    }

    void addMsgs(const MessagesList& msgs, bool reportAdded);

    void setSocket(SocketPtr socket);
    void setProtocol(ToolsProtocolPtr protocol);
    void addFilter(FilterPtr filter);

    typedef MsgMgr::MsgAddedCallbackFunc MsgAddedCallbackFunc;
    typedef MsgMgr::ErrorReportCallbackFunc ErrorReportCallbackFunc;
    typedef MsgMgr::SocketConnectionStatusReportCallbackFunc SocketConnectionStatusReportCallbackFunc;

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
    typedef unsigned long long MsgNumberType;
    typedef std::vector<FilterPtr> FiltersList;

    void socketDataReceived(DataInfoPtr dataInfoPtr);
    void updateInternalId(Message& msg);
    void reportMsgAdded(MessagePtr msg);
    void reportError(const QString& error);
    void reportSocketConnectionStatus(bool connected);

    AllMessages m_allMsgs;
    bool m_recvEnabled = false;

    SocketPtr m_socket;
    ToolsProtocolPtr m_protocol;
    FiltersList m_filters;
    MsgNumberType m_nextMsgNum = 1;
    bool m_running = false;

    MsgAddedCallbackFunc m_msgAddedCallback;
    ErrorReportCallbackFunc m_errorReportCallback;
    SocketConnectionStatusReportCallbackFunc m_socketConnectionStatusReportCallback;
};

}  // namespace cc_tools_qt

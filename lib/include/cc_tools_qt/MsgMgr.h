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

#include "cc_tools_qt/Api.h"
#include "cc_tools_qt/ToolsFilter.h"
#include "cc_tools_qt/Message.h"
#include "cc_tools_qt/ToolsProtocol.h"
#include "cc_tools_qt/ToolsSocket.h"

#include <memory>
#include <list>
#include <vector>


namespace cc_tools_qt
{

class MsgMgrImpl;
class CC_API MsgMgr
{
public:
    typedef std::list<MessagePtr> AllMessages;
    typedef ToolsProtocol::MessagesList MessagesList;

    typedef Message::Type MsgType;

    MsgMgr();
    ~MsgMgr() noexcept;

    void start();
    void stop();
    void clear();

    ToolsSocketPtr getSocket() const;
    ToolsProtocolPtr getProtocol() const;
    void setRecvEnabled(bool enabled);

    void deleteMsg(MessagePtr msg);
    void deleteMsgs(const MessagesList& msgs);
    void deleteAllMsgs();

    void sendMsgs(MessagesList&& msgs);

    const AllMessages& getAllMsgs() const;
    void addMsgs(const MessagesList& msgs, bool reportAdded = true);

    void setSocket(ToolsSocketPtr socket);
    void setProtocol(ToolsProtocolPtr protocol);
    void addFilter(ToolsFilterPtr filter);

    typedef std::function<void (MessagePtr msg)> MsgAddedCallbackFunc;
    typedef std::function<void (const QString& error)> ErrorReportCallbackFunc;
    typedef std::function<void (bool connected)> SocketConnectionStatusReportCallbackFunc;

    void setMsgAddedCallbackFunc(MsgAddedCallbackFunc&& func);
    void setErrorReportCallbackFunc(ErrorReportCallbackFunc&& func);
    void setSocketConnectionStatusReportCallbackFunc(SocketConnectionStatusReportCallbackFunc&& func);

private:
    std::unique_ptr<MsgMgrImpl> m_impl;
};

}  // namespace cc_tools_qt

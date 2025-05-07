//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsFilter.h"
#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsProtocol.h"
#include "cc_tools_qt/ToolsSocket.h"
#include "cc_tools_qt/version.h"

#include <memory>
#include <list>
#include <vector>


namespace cc_tools_qt
{

class ToolsMsgMgrImpl;
class CC_TOOLS_API ToolsMsgMgr
{
public:
    using MsgType = ToolsMessage::Type;

    ToolsMsgMgr();
    ~ToolsMsgMgr() noexcept;

    void start();
    void stop();
    void clear();

    ToolsSocketPtr getSocket() const;
    ToolsProtocolPtr getProtocol() const;
    void setRecvEnabled(bool enabled);

    void deleteMsg(ToolsMessagePtr msg);
    void deleteMsgs(const ToolsMessagesList& msgs);
    void deleteAllMsgs();

    void sendMsgs(ToolsMessagesList&& msgs);

    const ToolsMessagesList& getAllMsgs() const;
    void addMsgs(const ToolsMessagesList& msgs, bool reportAdded = true);

    void setSocket(ToolsSocketPtr socket);
    void setProtocol(ToolsProtocolPtr protocol);
    void addFilter(ToolsFilterPtr filter);

    using MsgAddedCallbackFunc = std::function<void (ToolsMessagePtr msg)>;
    using ErrorReportCallbackFunc = std::function<void (const QString& error)>;
    using SocketConnectionStatusReportCallbackFunc = std::function<void (bool connected)>;

    void setMsgAddedCallbackFunc(MsgAddedCallbackFunc&& func);
    void setErrorReportCallbackFunc(ErrorReportCallbackFunc&& func);
    void setSocketConnectionStatusReportCallbackFunc(SocketConnectionStatusReportCallbackFunc&& func);

private:
    std::unique_ptr<ToolsMsgMgrImpl> m_impl;
};

}  // namespace cc_tools_qt

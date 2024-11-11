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


#pragma once

#include <functional>

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsProtocol.h"

namespace cc_tools_qt
{

class ToolsMsgSendMgrImpl;
class CC_TOOLS_API ToolsMsgSendMgr
{
public:
    using SendMsgsCallbackFunc = std::function<void (ToolsMessagesList&&)>;
    using SendCompleteCallbackFunc = std::function<void ()>;

    ToolsMsgSendMgr();
    ~ToolsMsgSendMgr() noexcept;

    void setSendMsgsCallbackFunc(SendMsgsCallbackFunc&& func);
    void setSendCompeteCallbackFunc(SendCompleteCallbackFunc&& func);

    void start(ToolsProtocolPtr protocol, const ToolsMessagesList& msgs);

    void stop();

private:
    std::unique_ptr<ToolsMsgSendMgrImpl> m_impl;
};

}  // namespace cc_tools_qt



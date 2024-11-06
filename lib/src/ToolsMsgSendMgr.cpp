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

#include "cc_tools_qt/ToolsMsgSendMgr.h"

#include "ToolsMsgSendMgrImpl.h"

namespace cc_tools_qt
{

ToolsMsgSendMgr::ToolsMsgSendMgr()
  : m_impl(new ToolsMsgSendMgrImpl())
{
}

ToolsMsgSendMgr::~ToolsMsgSendMgr() noexcept = default;

void ToolsMsgSendMgr::setSendMsgsCallbackFunc(SendMsgsCallbackFunc&& func)
{
    m_impl->setSendMsgsCallbackFunc(std::move(func));
}

void ToolsMsgSendMgr::setSendCompeteCallbackFunc(SendCompleteCallbackFunc&& func)
{
    m_impl->setSendCompleteCallbackFunc(std::move(func));
}

void ToolsMsgSendMgr::start(ToolsProtocolPtr protocol, const MessagesList& msgs)
{
    m_impl->start(std::move(protocol), msgs);
}

void ToolsMsgSendMgr::stop()
{
    m_impl->stop();
}

}  // namespace cc_tools_qt


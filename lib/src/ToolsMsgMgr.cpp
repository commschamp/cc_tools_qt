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


#include "cc_tools_qt/ToolsMsgMgr.h"

#include "ToolsMsgMgrImpl.h"

namespace cc_tools_qt
{

ToolsMsgMgr::ToolsMsgMgr()
  : m_impl(new ToolsMsgMgrImpl())
{
}

ToolsMsgMgr::~ToolsMsgMgr() noexcept = default;

void ToolsMsgMgr::start()
{
    m_impl->start();
}

void ToolsMsgMgr::stop()
{
    m_impl->stop();
}

void ToolsMsgMgr::clear()
{
    m_impl->clear();
}

ToolsSocketPtr ToolsMsgMgr::getSocket() const
{
    return m_impl->getSocket();
}

ToolsProtocolPtr ToolsMsgMgr::getProtocol() const
{
    return m_impl->getProtocol();
}

void ToolsMsgMgr::setRecvEnabled(bool enabled)
{
    m_impl->setRecvEnabled(enabled);
}

void ToolsMsgMgr::deleteMsg(ToolsMessagePtr msg)
{
    m_impl->deleteMsg(std::move(msg));
}

void ToolsMsgMgr::deleteMsgs(const ToolsMessagesList& msgs)
{
    m_impl->deleteMsgs(msgs);
}

void ToolsMsgMgr::deleteAllMsgs()
{
    m_impl->deleteAllMsgs();
}

void ToolsMsgMgr::sendMsgs(ToolsMessagesList&& msgs)
{
    m_impl->sendMsgs(std::move(msgs));
}

const ToolsMessagesList& ToolsMsgMgr::getAllMsgs() const
{
    return m_impl->getAllMsgs();
}

void ToolsMsgMgr::addMsgs(const ToolsMessagesList& msgs, bool reportAdded)
{
    m_impl->addMsgs(msgs, reportAdded);
}

void ToolsMsgMgr::setSocket(ToolsSocketPtr socket)
{
    m_impl->setSocket(std::move(socket));
}

void ToolsMsgMgr::setProtocol(ToolsProtocolPtr protocol)
{
    m_impl->setProtocol(std::move(protocol));
}

void ToolsMsgMgr::addFilter(ToolsFilterPtr filter)
{
    m_impl->addFilter(std::move(filter));
}

void ToolsMsgMgr::setMsgAddedCallbackFunc(MsgAddedCallbackFunc&& func)
{
    m_impl->setMsgAddedCallbackFunc(std::move(func));
}

void ToolsMsgMgr::setErrorReportCallbackFunc(ErrorReportCallbackFunc&& func)
{
    m_impl->setErrorReportCallbackFunc(std::move(func));
}

void ToolsMsgMgr::setSocketConnectionStatusReportCallbackFunc(
    SocketConnectionStatusReportCallbackFunc&& func)
{
    m_impl->setSocketConnectionStatusReportCallbackFunc(std::move(func));
}

}  // namespace cc_tools_qt


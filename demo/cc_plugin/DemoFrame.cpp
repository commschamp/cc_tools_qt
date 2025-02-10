//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "DemoFrame.h"

#include "DemoMessage.h"
#include "DemoMsgFactory.h"
#include "DemoTransportMessage.h"

#include "demo/Frame.h"

#include "cc_tools_qt/ToolsFrameBase.h"

namespace demo
{

namespace cc_plugin
{

class DemoFrameImpl : public 
    cc_tools_qt::ToolsFrameBase<
        DemoMessage, 
        demo::Frame<DemoMessage::ProtInterface>, 
        DemoMsgFactory, 
        DemoTransportMessage
    >
{
}; 

DemoFrame::DemoFrame() : 
    m_pImpl(new DemoFrameImpl)
{
}

DemoFrame::~DemoFrame() = default;

cc_tools_qt::ToolsMessagesList DemoFrame::readDataImpl(const cc_tools_qt::ToolsDataInfo& dataInfo, bool final)
{
    return m_pImpl->readData(dataInfo, final);
}

void DemoFrame::updateMessageImpl(cc_tools_qt::ToolsMessage& msg)
{
    m_pImpl->updateMessage(msg);
}

cc_tools_qt::ToolsMessagePtr DemoFrame::createInvalidMessageImpl()
{
    return m_pImpl->createInvalidMessage();
}

cc_tools_qt::ToolsMessagePtr DemoFrame::createRawDataMessageImpl()
{
    return m_pImpl->createRawDataMessage();
}

cc_tools_qt::ToolsMessagePtr DemoFrame::createExtraInfoMessageImpl()
{
    return m_pImpl->createExtraInfoMessage();
}

cc_tools_qt::ToolsMessagesList DemoFrame::createAllMessagesImpl()
{
    return m_pImpl->createAllMessages();
}

cc_tools_qt::ToolsMessagePtr DemoFrame::createMessageImpl(const QString& idAsString, unsigned idx)
{
    return m_pImpl->createMessage(idAsString, idx);
}

DemoFrame::DataSeq DemoFrame::writeProtMsgImpl(const void* protInterface)
{
    return m_pImpl->writeProtMsg(protInterface);
}

}  // namespace cc_plugin

}  // namespace demo


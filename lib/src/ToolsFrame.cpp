//
// Copyright 2024 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsFrame.h"

#include <algorithm>
#include <cassert>
#include <iterator>

namespace cc_tools_qt
{

ToolsFrame::~ToolsFrame() = default;

ToolsMessagesList ToolsFrame::readData(const ToolsDataInfo& dataInfo, bool final)
{
    return readDataImpl(dataInfo, final);
}

void ToolsFrame::updateMessage(ToolsMessage& msg)
{
    updateMessageImpl(msg);
}

ToolsMessagePtr ToolsFrame::createInvalidMessage()
{
    return createInvalidMessageImpl();
}

ToolsMessagePtr ToolsFrame::createRawDataMessage()
{
    return createRawDataMessageImpl();
}

ToolsMessagePtr ToolsFrame::createExtraInfoMessage()
{
    return createExtraInfoMessageImpl();
}

ToolsMessagesList ToolsFrame::createAllMessages()
{
    return createAllMessagesImpl();
}

ToolsMessagePtr ToolsFrame::createMessage(const QString& idAsString, unsigned idx)
{
    return createMessageImpl(idAsString, idx);
}

ToolsFrame::DataSeq ToolsFrame::writeProtMsg(const void* protInterface)
{
    return writeProtMsgImpl(protInterface);
}

}  // namespace cc_tools_qt


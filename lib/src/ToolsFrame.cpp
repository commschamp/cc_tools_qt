//
// Copyright 2024 - 2024 (C). Alex Robenko. All rights reserved.
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

ToolsFrame::MessagesList ToolsFrame::readData(const DataInfo& dataInfo, bool final)
{
    return readDataImpl(dataInfo, final);
}

void ToolsFrame::updateMessage(Message& msg)
{
    updateMessageImpl(msg);
}

MessagePtr ToolsFrame::createInvalidMessage()
{
    return createInvalidMessageImpl();
}

MessagePtr ToolsFrame::createRawDataMessage()
{
    return createRawDataMessageImpl();
}

MessagePtr ToolsFrame::createExtraInfoMessage()
{
    return createExtraInfoMessageImpl();
}

ToolsFrame::MessagesList ToolsFrame::createAllMessages()
{
    return createAllMessagesImpl();
}

MessagePtr ToolsFrame::createMessage(const QString& idAsString, unsigned idx)
{
    return createMessageImpl(idAsString, idx);
}

}  // namespace cc_tools_qt


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


#pragma once

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/DataInfo.h"
#include "cc_tools_qt/Message.h"

#include <memory>
#include <list>

namespace cc_tools_qt
{

class CC_TOOLS_API ToolsFrame
{
public:
    using MessagesList = std::list<MessagePtr>;

    virtual ~ToolsFrame();

    MessagesList readData(const DataInfo& dataInfo, bool final);
    void updateMessage(Message& msg);
    MessagePtr createInvalidMessage();
    MessagePtr createRawDataMessage();
    MessagePtr createExtraInfoMessage();
    MessagesList createAllMessages();
    MessagePtr createMessage(const QString& idAsString, unsigned idx);

protected:
    ToolsFrame() = default;

    virtual MessagesList readDataImpl(const DataInfo& dataInfo, bool final) = 0;
    virtual void updateMessageImpl(Message& msg) = 0;
    virtual MessagePtr createInvalidMessageImpl() = 0;
    virtual MessagePtr createRawDataMessageImpl() = 0;
    virtual MessagePtr createExtraInfoMessageImpl() = 0;
    virtual MessagesList createAllMessagesImpl() = 0;
    virtual MessagePtr createMessageImpl(const QString& idAsString, unsigned idx) = 0;
};

using ToolsFramePtr = std::unique_ptr<ToolsFrame>;

}  // namespace cc_tools_qt



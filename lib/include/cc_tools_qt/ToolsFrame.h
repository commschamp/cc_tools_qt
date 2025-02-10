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


#pragma once

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsDataInfo.h"
#include "cc_tools_qt/ToolsMessage.h"

#include <memory>
#include <list>

namespace cc_tools_qt
{

class CC_TOOLS_API ToolsFrame
{
public:
    using DataSeq = ToolsMessage::DataSeq;

    virtual ~ToolsFrame();

    ToolsMessagesList readData(const ToolsDataInfo& dataInfo, bool final);
    void updateMessage(ToolsMessage& msg);
    ToolsMessagePtr createInvalidMessage();
    ToolsMessagePtr createRawDataMessage();
    ToolsMessagePtr createExtraInfoMessage();
    ToolsMessagesList createAllMessages();
    ToolsMessagePtr createMessage(const QString& idAsString, unsigned idx);
    DataSeq writeProtMsg(const void* protInterface);

protected:
    ToolsFrame() = default;

    virtual ToolsMessagesList readDataImpl(const ToolsDataInfo& dataInfo, bool final) = 0;
    virtual void updateMessageImpl(ToolsMessage& msg) = 0;
    virtual ToolsMessagePtr createInvalidMessageImpl() = 0;
    virtual ToolsMessagePtr createRawDataMessageImpl() = 0;
    virtual ToolsMessagePtr createExtraInfoMessageImpl() = 0;
    virtual ToolsMessagesList createAllMessagesImpl() = 0;
    virtual ToolsMessagePtr createMessageImpl(const QString& idAsString, unsigned idx) = 0;
    virtual DataSeq writeProtMsgImpl(const void* protInterface) = 0;
};

using ToolsFramePtr = std::unique_ptr<ToolsFrame>;

}  // namespace cc_tools_qt



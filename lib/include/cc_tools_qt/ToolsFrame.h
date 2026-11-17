//
// Copyright 2024 - 2026 (C). Alex Robenko. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later
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
#include "cc_tools_qt/version.h"

#include <memory>
#include <list>

namespace cc_tools_qt
{

class CC_TOOLS_API ToolsFrame
{
public:
    using DataSeq = ToolsMessage::DataSeq;

    virtual ~ToolsFrame();

    [[nodiscard]] ToolsMessagesList readData(const ToolsDataInfo& dataInfo, bool final);
    void updateMessage(ToolsMessage& msg);
    [[nodiscard]] ToolsMessagePtr createInvalidMessage();
    [[nodiscard]] ToolsMessagePtr createRawDataMessage();
    [[nodiscard]] ToolsMessagePtr createExtraInfoMessage();
    [[nodiscard]] ToolsMessagesList createAllMessages();
    [[nodiscard]] ToolsMessagePtr createMessage(const QString& idAsString, unsigned idx);
    [[nodiscard]] DataSeq writeProtMsg(const void* protInterface);

protected:
    ToolsFrame() = default;

    [[nodiscard]] virtual ToolsMessagesList readDataImpl(const ToolsDataInfo& dataInfo, bool final) = 0;
    virtual void updateMessageImpl(ToolsMessage& msg) = 0;
    [[nodiscard]] virtual ToolsMessagePtr createInvalidMessageImpl() = 0;
    [[nodiscard]] virtual ToolsMessagePtr createRawDataMessageImpl() = 0;
    [[nodiscard]] virtual ToolsMessagePtr createExtraInfoMessageImpl() = 0;
    [[nodiscard]] virtual ToolsMessagesList createAllMessagesImpl() = 0;
    [[nodiscard]] virtual ToolsMessagePtr createMessageImpl(const QString& idAsString, unsigned idx) = 0;
    [[nodiscard]] virtual DataSeq writeProtMsgImpl(const void* protInterface) = 0;
};

using ToolsFramePtr = std::unique_ptr<ToolsFrame>;

}  // namespace cc_tools_qt


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

#include "cc_tools_qt/Message.h"

#include <vector>

namespace cc_tools_qt
{

class ToolsMsgFactory
{
public:
    using MessageList = std::vector<MessagePtr>;

    virtual ~ToolsMsgFactory();

    MessagePtr createMessage(const QString& idAsString, unsigned idx = 0);
    MessagePtr createMessage(const qlonglong id, unsigned idx = 0);
    MessageList createAllMessages();

protected:
    ToolsMsgFactory();
    virtual MessageList createAllMessagesImpl() = 0;

private:
    void createDefaultMessagesIfNeeded();

    mutable MessageList m_defaultMsgs;  
};

}  // namespace cc_tools_qt



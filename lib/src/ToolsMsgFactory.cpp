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


#include "cc_tools_qt/ToolsMsgFactory.h"

#include <algorithm>
#include <cassert>
#include <iterator>

namespace cc_tools_qt
{
ToolsMsgFactory::~ToolsMsgFactory() = default;

ToolsMessagePtr ToolsMsgFactory::createMessage(const QString& idAsString, unsigned idx)
{
    bool ok = false;
    auto numId = idAsString.toLongLong(&ok, 10);
    do {
        if (ok) {
            break;
        }

        numId = idAsString.toLongLong(&ok, 16);
        if (ok) {
            break;
        }

        return ToolsMessagePtr();
    } while (false);

    return createMessage(numId, idx);
}

ToolsMessagePtr ToolsMsgFactory::createMessage(const qlonglong id, unsigned idx)
{
    createDefaultMessagesIfNeeded();
    auto lowerIter = 
        std::lower_bound(
            m_defaultMsgs.begin(), m_defaultMsgs.end(), id,
            [](auto& msg, qlonglong idParam)
            {
                return msg->numericId() < idParam;
            });

    if ((lowerIter == m_defaultMsgs.end()) ||
        ((*lowerIter)->numericId() != id)) {
        return ToolsMessagePtr();
    }

    auto upperIter = 
        std::upper_bound(
            m_defaultMsgs.begin(), m_defaultMsgs.end(), id,
            [](qlonglong idParam, auto& msg)
            {
                return idParam < msg->numericId();
            });

    auto distance = static_cast<unsigned>(std::distance(lowerIter, upperIter));
    if (distance <= idx) {
        return ToolsMessagePtr();
    }

    auto iter = lowerIter + idx;
    assert(iter != m_defaultMsgs.end());
    return (*iter)->clone();
}

ToolsMessagesList ToolsMsgFactory::createAllMessages()
{
    createDefaultMessagesIfNeeded();
    ToolsMessagesList result;
    for (auto& m : m_defaultMsgs) {
        assert(m);
        result.push_back(m->clone());
    }
    return result;
}

ToolsMsgFactory::ToolsMsgFactory() = default;

void ToolsMsgFactory::createDefaultMessagesIfNeeded()
{
    if (m_defaultMsgs.empty()) {
        m_defaultMsgs = createAllMessagesImpl();
    }
}

}  // namespace cc_tools_qt


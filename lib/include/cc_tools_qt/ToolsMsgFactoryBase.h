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

#include "cc_tools_qt/ToolsMsgFactory.h"

#include "comms/util/Tuple.h"

#include <tuple>

namespace cc_tools_qt
{

namespace details
{

class TupleMsgFactoryHelper
{
public:
    using MessagesListInternal = std::vector<ToolsMessagePtr>;
    TupleMsgFactoryHelper(MessagesListInternal& msgs) : m_msgs(msgs) {}

    template <typename TMsg>
    void operator()()
    {
        m_msgs.push_back(cc_tools_qt::ToolsMessagePtr(new TMsg));
    }

    MessagesListInternal& msgs()
    {
        return m_msgs;
    }

private:
    MessagesListInternal& m_msgs;
};

}  // namespace details

template <typename TAllMessages>
class ToolsMsgFactoryBase : public ToolsMsgFactory
{
public:
    using AllMessages = TAllMessages;

protected:
    virtual MessagesListInternal createAllMessagesImpl() override
    {
        MessagesListInternal result;
        static_assert(comms::util::IsTuple<TAllMessages>::Value, "TAllMessages must be std::tuple");
        result.reserve(std::tuple_size<TAllMessages>::value);
        comms::util::tupleForEachType<TAllMessages>(details::TupleMsgFactoryHelper(result));
        return result;
    }
};

}  // namespace cc_tools_qt


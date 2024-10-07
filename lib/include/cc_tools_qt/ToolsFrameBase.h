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

#include "cc_tools_qt/ToolsFrameCommon.h"

#include <cassert>

namespace cc_tools_qt
{

template <typename TMsgBase, typename TProtFrame, typename TMsgFactory>
class ToolsFrameBase : public ToolsFrameCommon<TMsgBase>
{
    using Base = ToolsFrameCommon<TMsgBase>;
public:
    using MessageList = typename Base::MessageList;
    using ProtMsgBase = typename Base::ProtMsgBase;

    ToolsFrameBase() = default;

protected:
    virtual MessageList readDataImpl(const DataInfo& dataInfo, bool final) override
    {
        m_inData.reserve(m_inData.size() + dataInfo.m_data.size());
        m_inData.insert(m_inData.end(), dataInfo.m_data.begin(), dataInfo.m_data.end());

        MessageList result;
        std::size_t consumed = 0U;


        using ProtMsgPtr = typename TProtFrame::MsgPtr;
        using ReadIter = typename ProtMsgBase::ReadIterator;
        while (consumed < m_inData.size()) {
            ProtMsgPtr msgPtr;

            ReadIter readIter = m_inData.data() + consumed;
            auto remLen = m_inData.size() - consumed;
            assert(0U < remLen);

            qlonglong msgId = 0;
            std::size_t idx = 0;
            auto es = 
                m_frame.read(
                    msgPtr,
                    readIter,
                    remLen,
                    comms::protocol::msgId(msgId),
                    comms::protocol::msgIndex(idx));

            if (es == comms::ErrorStatus::NotEnoughData) {
                break;
            }

        }

        static_cast<void>(final);
        return result;
    }

private:
    using DataSeq = DataInfo::DataSeq;
    TProtFrame m_frame;
    TMsgFactory m_factory;
    DataSeq m_inData;
    DataSeq m_garbage;
};

}  // namespace cc_tools_qt



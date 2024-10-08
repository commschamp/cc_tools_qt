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

#include "cc_tools_qt/ToolsExtraInfoMessage.h"
#include "cc_tools_qt/ToolsFrameCommon.h"
#include "cc_tools_qt/ToolsInvalidMessage.h"
#include "cc_tools_qt/ToolsRawDataMessage.h"
#include "cc_tools_qt/property/message.h"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QByteArray>

#include <algorithm>
#include <cassert>
#include <iterator>

namespace cc_tools_qt
{

template <typename TMsgBase, typename TProtFrame, typename TMsgFactory, typename TTransportMsg>
class ToolsFrameBase : public ToolsFrameCommon<TMsgBase>
{
    using Base = ToolsFrameCommon<TMsgBase>;
public:
    using MessageList = typename Base::MessageList;
    using ProtMsgBase = typename Base::ProtMsgBase;

    using TransportMsg = TTransportMsg;

    /// @brief Type of "Invalid Message".
    using InvalidMsg = ToolsInvalidMessage<TMsgBase>;

    using RawDataMsg = ToolsRawDataMessage<TMsgBase>;

    /// @brief Type of "Extra Info Message"
    using ExtraInfoMsg = ToolsExtraInfoMessage<TMsgBase>;    

    ToolsFrameBase() = default;

protected:
    virtual MessageList readDataImpl(const DataInfo& dataInfo, bool final) override
    {
        m_inData.reserve(m_inData.size() + dataInfo.m_data.size());
        m_inData.insert(m_inData.end(), dataInfo.m_data.begin(), dataInfo.m_data.end());

        MessageList allMsgs;
        std::size_t consumed = 0U;

        auto checkGarbageFunc =
            [this, &allMsgs]()
            {
                if (m_garbage.empty()) {
                    return;
                }

                MessagePtr rawDataMsg(new RawDataMsg);
                if (!rawDataMsg->decodeData(m_garbage)) {
                    [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
                    assert(Must_not_be_happen); 
                }
                    
                MessagePtr invalidMsgPtr(new InvalidMsg);
                property::message::RawDataMsg().setTo(std::move(rawDataMsg), *invalidMsgPtr);
                allMsgs.push_back(std::move(invalidMsgPtr));
                m_garbage.clear();
            };


        using ProtMsgPtr = typename TProtFrame::MsgPtr;
        using ReadIter = typename ProtMsgBase::ReadIterator;
        while (consumed < m_inData.size()) {
            ProtMsgPtr msgPtr;

            ReadIter readIterBeg = m_inData.data() + consumed;
            ReadIter readIter = readIterBeg;
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

            if (es == comms::ErrorStatus::MsgAllocFailure) {
                [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
                assert(Must_not_be_happen); 
                break;
            }            

            if (es != comms::ErrorStatus::Success) {
                m_garbage.push_back(*readIterBeg);
                static constexpr std::size_t GarbageLimit = 512;
                if (GarbageLimit <= m_garbage.size()) {
                    checkGarbageFunc();
                }

                ++consumed; 
                continue;               
            }

            checkGarbageFunc();
            assert(msgPtr);
            auto diff = static_cast<std::size_t>(std::distance(readIterBeg, readIter));
            consumed += diff;

            auto toolsMsg = m_factory.createMessage(msgId, static_cast<unsigned>(idx));
            if (!toolsMsg) {
                [[maybe_unused]] static constexpr bool Protocol_and_Tools_Frames_Out_of_Sync = false;
                assert(Protocol_and_Tools_Frames_Out_of_Sync);
                
                m_garbage.reserve(m_garbage.size() + diff);
                m_garbage.insert(m_garbage.end(), readIterBeg, readIter);
                checkGarbageFunc();
                continue;
            }

            toolsMsg->assignProtMessage(msgPtr.get());

            DataSeq data(readIterBeg, readIter);

            MessagePtr transportMsg(new TransportMsg);
            if (!transportMsg->decodeData(data)) {
                [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
                assert(Must_not_be_happen);                
            }

            MessagePtr rawDataMsg(new RawDataMsg);
            if (!rawDataMsg->decodeData(data)) {
                [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
                assert(Must_not_be_happen); 
            }    
            
            property::message::TransportMsg().setTo(std::move(transportMsg), *toolsMsg);        
            property::message::RawDataMsg().setTo(std::move(rawDataMsg), *toolsMsg);        
            allMsgs.push_back(std::move(toolsMsg));
        }

        static_cast<void>(final);
        
        assert(consumed <= m_inData.size());
        m_inData.erase(m_inData.begin(), m_inData.begin() + consumed);

        if (final && (!m_inData.empty())) {
            m_garbage.reserve(m_garbage.size() + m_inData.size());
            m_garbage.insert(m_garbage.end(), m_inData.begin(), m_inData.end());
            m_inData.clear();
            checkGarbageFunc();
        }        

        if (!dataInfo.m_extraProperties.isEmpty()) {
            auto jsonObj = QJsonObject::fromVariantMap(dataInfo.m_extraProperties);
            QJsonDocument doc(jsonObj);
            auto jsonData = doc.toJson();
            DataSeq jsonRawBytes(jsonData.begin(), jsonData.end());

            for (auto& m : allMsgs) {
                property::message::ExtraInfo().setTo(dataInfo.m_extraProperties, *m);

                MessagePtr extraInfoMsg(new ExtraInfoMsg);
                if (!extraInfoMsg->decodeData(jsonRawBytes)) {
                    [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
                    assert(Must_not_be_happen); 
                    continue;                    
                }

                property::message::ExtraInfoMsg().setTo(std::move(extraInfoMsg), *m);        
            }
        }

        return allMsgs;
    }

private:
    using DataSeq = DataInfo::DataSeq;
    TProtFrame m_frame;
    TMsgFactory m_factory;
    DataSeq m_inData;
    DataSeq m_garbage;
};

}  // namespace cc_tools_qt



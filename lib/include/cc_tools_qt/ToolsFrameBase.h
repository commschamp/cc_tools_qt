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

template <typename TMsgBase, template<typename...> class TProtFrame, typename TMsgFactory, typename TTransportMsg>
class ToolsFrameBase : public ToolsFrameCommon<TMsgBase>
{
    using Base = ToolsFrameCommon<TMsgBase>;
public:
    using MessagesList = typename Base::MessagesList;
    using ProtMsgBase = typename Base::ProtMsgBase;
    using DataSeq = typename Base::DataSeq;

    using TransportMsg = TTransportMsg;

    /// @brief Type of "Invalid ToolsMessage".
    using InvalidMsg = ToolsInvalidMessage<TMsgBase>;

    using RawDataMsg = ToolsRawDataMessage<TMsgBase>;

    /// @brief Type of "Extra Info ToolsMessage"
    using ExtraInfoMsg = ToolsExtraInfoMessage<TMsgBase>;

    using ProtFrame = TProtFrame<ProtMsgBase>;

    ToolsFrameBase() = default;

protected:
    virtual MessagesList readDataImpl(const DataInfo& dataInfo, bool final) override
    {
        m_inData.reserve(m_inData.size() + dataInfo.m_data.size());
        m_inData.insert(m_inData.end(), dataInfo.m_data.begin(), dataInfo.m_data.end());

        MessagesList allMsgs;
        std::size_t consumed = 0U;

        auto checkGarbageFunc =
            [this, &allMsgs]()
            {
                if (m_garbage.empty()) {
                    return;
                }

                ToolsMessagePtr invalidMsgPtr(new InvalidMsg);
                updateRawDataInternal(m_garbage, *invalidMsgPtr);
                allMsgs.push_back(std::move(invalidMsgPtr));
                m_garbage.clear();
            };


        using ProtMsgPtr = typename ProtFrame::MsgPtr;
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
            updateTransportInternal(data, *toolsMsg);
            updateRawDataInternal(data, *toolsMsg);
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
                updateExtraInfoInternal(jsonRawBytes, *m);
            }
        }

        return allMsgs;
    }

    virtual void updateMessageImpl(ToolsMessage& msg) override
    {
        auto data = msg.encodeFramed(*this);
        updateTransportInternal(data, msg);
        updateRawDataInternal(data, msg);

        auto extraProps = property::message::ExtraInfo().getFrom(msg);
        bool extraInfoMsgIsForced = property::message::ForceExtraInfoExistence().getFrom(msg);
        if (extraProps.isEmpty() && (!extraInfoMsgIsForced)) {
            property::message::ExtraInfoMsg().setTo(ToolsMessagePtr(), msg);
            return;
        }

        auto extraInfoMsgPtr = std::make_unique<ExtraInfoMsg>();
        if (extraProps.isEmpty()) {
            property::message::ExtraInfoMsg().setTo(ToolsMessagePtr(extraInfoMsgPtr.release()), msg);
            return;
        }        

        auto jsonObj = QJsonObject::fromVariantMap(extraProps);
        QJsonDocument doc(jsonObj);
        auto jsonData = doc.toJson();
        DataSeq jsonRawBytes(jsonData.begin(), jsonData.end());
        updateExtraInfoInternal(jsonRawBytes, msg);
    }

    virtual ToolsMessagePtr createInvalidMessageImpl() override
    {
        return ToolsMessagePtr(new InvalidMsg());
    }

    virtual ToolsMessagePtr createRawDataMessageImpl() override
    {
        return ToolsMessagePtr(new RawDataMsg());
    }    

    virtual ToolsMessagePtr createExtraInfoMessageImpl() override
    {
        return ToolsMessagePtr(new ExtraInfoMsg());
    }

    virtual MessagesList createAllMessagesImpl() override
    {
        return m_factory.createAllMessages();
    }

    virtual ToolsMessagePtr createMessageImpl(const QString& idAsString, unsigned idx) override
    {
        return m_factory.createMessage(idAsString, idx);
    }

    virtual DataSeq writeProtMsgImpl(const ProtMsgBase& msg) override
    {
        DataSeq data;
        data.reserve(m_frame.length(msg));

        auto writeIter = std::back_inserter(data);
        auto es = m_frame.write(msg, writeIter, data.max_size());
        if (es == comms::ErrorStatus::UpdateRequired) {
            auto updateIter = data.data();
            es = m_frame.update(msg, updateIter, data.size());
        }

        if (es != comms::ErrorStatus::Success) {
            [[maybe_unused]] static constexpr bool Unexpected_write_update_failure = false;
            assert(Unexpected_write_update_failure); 
            data.clear();
        }

        return data;
    }

private:
    void updateTransportInternal(const DataSeq& data, ToolsMessage& msg)
    {
        ToolsMessagePtr transportMsg(new TransportMsg);
        if (!transportMsg->decodeData(data)) {
            [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
            assert(Must_not_be_happen);                
        }

        property::message::TransportMsg().setTo(std::move(transportMsg), msg);
    }

    void updateRawDataInternal(const DataSeq& data, ToolsMessage& msg)
    {
        ToolsMessagePtr rawDataMsg(new RawDataMsg);
        if (!rawDataMsg->decodeData(data)) {
            [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
            assert(Must_not_be_happen); 
        }    
        
        property::message::RawDataMsg().setTo(std::move(rawDataMsg), msg);   
    }    

    void updateExtraInfoInternal(const DataSeq& jsonRawBytes, ToolsMessage& msg)
    {
        ToolsMessagePtr extraInfoMsg(new ExtraInfoMsg);
        if (!extraInfoMsg->decodeData(jsonRawBytes)) {
            [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
            assert(Must_not_be_happen); 
        }

        property::message::ExtraInfoMsg().setTo(std::move(extraInfoMsg), msg);        
  
    }     

    ProtFrame m_frame;
    TMsgFactory m_factory;
    DataSeq m_inData;
    DataSeq m_garbage;
};

}  // namespace cc_tools_qt



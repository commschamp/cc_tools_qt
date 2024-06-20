//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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

#include <algorithm>
#include <iterator>
#include <cassert>


#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QByteArray>

#include "cc_tools_qt/ErrorStatus.h"
#include "comms/util/ScopeGuard.h"
#include "comms/util/Tuple.h"

#include "Protocol.h"
#include "Message.h"
#include "RawDataMessage.h"
#include "InvalidMessage.h"
#include "ExtraInfoMessage.h"
#include "cc_tools_qt.h"

namespace cc_tools_qt
{

/// @brief Helper class to define custom @ref Protocol
/// @details Provides the default implementation to most of the virtual
///     functions defined by @ref Protocol class.
/// @tparam TProtStack Definition of the protocol stack.
/// @tparam TTransportMsg Definition of the "Transport Message".
/// @tparam TRawDataMsg Definition of the "Raw Data Message", defaults to
///     @ref RawDataMessage
/// @headerfile cc_tools_qt/ProtocolBase.h
template <
    typename TProtStack,
    typename TTransportMsg,
    typename TRawDataMsg = RawDataMessage<TProtStack> >
class ProtocolBase : public Protocol
{
protected:
    /// @brief Default constructor
    ProtocolBase() = default;

    /// @brief Definition of "protocol stack" type.
    using ProtocolStack = TProtStack;

    /// @brief Definition of "Transport Message" type.
    using TransportMsg = TTransportMsg;

    /// @brief Definition of "Raw Data Message" type
    using RawDataMsg = TRawDataMsg;

    /// @brief Definition of the pointer to message object
    using ProtocolMsgPtr = typename ProtocolStack::MsgPtr;
    static_assert(!std::is_void<ProtocolMsgPtr>::value,
        "ProtocolStack does not define MsgPtr");

    /// @brief Type of the common interface class
    using ProtocolMessage = typename ProtocolMsgPtr::element_type;

    /// @brief Type used to represent message ID.
    using MsgIdType = typename ProtocolMessage::MsgIdType;

    /// @brief Type of message ID when passed as a parameter.
    using MsgIdParamType = typename ProtocolMessage::MsgIdParamType;

    /// @brief All messages bundle (@b std::tuple)
    /// @details Taken from "protocol stack" definition.
    using AllMessages = typename ProtocolStack::AllMessages;

    /// @brief Type of "Invalid Message".
    using InvalidMsg = InvalidMessage<ProtocolMessage>;

    /// @brief Type of "Extra Info Message"
    using ExtraInfoMsg = ExtraInfoMessage<ProtocolMessage>;

    /// @brief Type of message factory
    using MsgFactory = typename TProtStack::MsgFactory;

    static_assert(
        !std::is_void<AllMessages>::value,
        "AllMessages must be a normal type");

    static_assert(
        comms::util::IsTuple<AllMessages>::Value,
        "AllMessages is expected to be a tuple.");

    /// @brief Overriding implementation to Protocol::readImpl().
    virtual MessagesList readImpl(const DataInfo& dataInfo, bool final) override
    {
        const std::uint8_t* iter = &dataInfo.m_data[0];
        auto size = dataInfo.m_data.size();

        MessagesList allMsgs;
        m_data.reserve(m_data.size() + size);
        std::copy_n(iter, size, std::back_inserter(m_data));

        using ReadIterator = typename ProtocolMessage::ReadIterator;
        ReadIterator readIterBeg = &m_data[0];

        auto remainingSizeCalc =
            [this](ReadIterator readIter) -> std::size_t
            {
                ReadIterator const dataBegin = &m_data[0];
                auto consumed =
                    static_cast<std::size_t>(
                        std::distance(dataBegin, readIter));
                assert(consumed <= m_data.size());
                return m_data.size() - consumed;
            };

        auto eraseGuard =
            comms::util::makeScopeGuard(
                [this, &readIterBeg]()
                {
                    ReadIterator dataBegin = &m_data[0];
                    auto dist = std::distance(dataBegin, readIterBeg);
                    m_data.erase(m_data.begin(), m_data.begin() + dist);
                });

        auto setExtraInfoFunc =
            [&dataInfo](Message& msg)
            {
                if (dataInfo.m_extraProperties.isEmpty()) {
                    return;
                }

                auto jsonObj = QJsonObject::fromVariantMap(dataInfo.m_extraProperties);
                QJsonDocument doc(jsonObj);

                std::unique_ptr<ExtraInfoMsg> extraInfoMsgPtr(new ExtraInfoMsg());
                auto& str = std::get<0>(extraInfoMsgPtr->fields());
                str.value() = doc.toJson().constData();
                setExtraInfoToMessageProperties(dataInfo.m_extraProperties, msg);
                setExtraInfoMsgToMessageProperties(
                    MessagePtr(extraInfoMsgPtr.release()),
                    msg);
            };

        auto checkGarbageFunc =
            [this, &allMsgs, &setExtraInfoFunc]()
            {
                if (!m_garbage.empty()) {
                    MessagePtr invalidMsgPtr(new InvalidMsg());
                    setNameToMessageProperties(*invalidMsgPtr);
                    std::unique_ptr<RawDataMsg> rawDataMsgPtr(new RawDataMsg());
                    ReadIterator garbageReadIterator = &m_garbage[0];
                    [[maybe_unused]] auto esTmp = rawDataMsgPtr->read(garbageReadIterator, m_garbage.size());
                    assert(esTmp == comms::ErrorStatus::Success);
                    setRawDataToMessageProperties(MessagePtr(rawDataMsgPtr.release()), *invalidMsgPtr);
                    setExtraInfoFunc(*invalidMsgPtr);
                    allMsgs.push_back(std::move(invalidMsgPtr));
                    m_garbage.clear();
                }
            };

        while (true) {
            ProtocolMsgPtr msgPtr;

            auto readIterCur = readIterBeg;
            auto remainingSize = remainingSizeCalc(readIterCur);
            if (remainingSize == 0U) {
                break;
            }

            auto es =
                m_protStack.read(
                    msgPtr,
                    readIterCur,
                    remainingSize);

            if (es == comms::ErrorStatus::NotEnoughData) {
                break;
            }

            auto addMsgInfoGuard =
                comms::util::makeScopeGuard(
                    [this, &allMsgs, &msgPtr]()
                    {
                        assert(msgPtr);
                        setNameToMessageProperties(*msgPtr);
                        allMsgs.push_back(MessagePtr(std::move(msgPtr)));
                    });

            auto setExtrasFunc =
                [readIterBeg, &readIterCur, &msgPtr, &setExtraInfoFunc]()
                {
                    // readIterBeg is captured by value on purpose
                    auto dataSize = static_cast<std::size_t>(
                                std::distance(readIterBeg, readIterCur));

                    auto readTransportIterBegTmp = readIterBeg;
                    std::unique_ptr<TransportMsg> transportMsgPtr(new TransportMsg());
                    [[maybe_unused]] auto esTmp = transportMsgPtr->read(readTransportIterBegTmp, dataSize);
                    assert(esTmp == comms::ErrorStatus::Success);
                    setTransportToMessageProperties(MessagePtr(transportMsgPtr.release()), *msgPtr);

                    auto readRawIterBegTmp = readIterBeg;
                    std::unique_ptr<RawDataMsg> rawDataMsgPtr(new RawDataMsg());
                    esTmp = rawDataMsgPtr->read(readRawIterBegTmp, dataSize);
                    assert(esTmp == comms::ErrorStatus::Success);
                    setRawDataToMessageProperties(MessagePtr(rawDataMsgPtr.release()), *msgPtr);
                    setExtraInfoFunc(*msgPtr);
                };

            if (es == comms::ErrorStatus::Success) {
                checkGarbageFunc();
                assert(msgPtr);
                setExtrasFunc();
                readIterBeg = readIterCur;
                continue;
            }

            if (es == comms::ErrorStatus::InvalidMsgData) {
                checkGarbageFunc();
                msgPtr.reset(new InvalidMsg());
                setExtrasFunc();
                readIterBeg = readIterCur;
                continue;
            }

            addMsgInfoGuard.release();

            if (es == comms::ErrorStatus::MsgAllocFailure) {
                [[maybe_unused]] static constexpr bool Must_not_be_happen = false;
                assert(Must_not_be_happen); 
                break;
            }

            // Protocol error
            m_garbage.push_back(*readIterBeg);
            static const std::size_t GarbageLimit = 512;
            if (GarbageLimit <= m_garbage.size()) {
                checkGarbageFunc();
            }
            ++readIterBeg;
        }

        if (final) {
            ReadIterator dataBegin = &m_data[0];
            auto consumed = std::distance(dataBegin, readIterBeg);
            auto remDataCount = static_cast<decltype(consumed)>(m_data.size()) - consumed;
            m_garbage.insert(m_garbage.end(), m_data.begin() + consumed, m_data.end());
            std::advance(readIterBeg, remDataCount);
            checkGarbageFunc();
        }
        return allMsgs;
    }

    /// @brief Overriding implementation to Protocol::writeImpl().
    virtual DataInfoPtr writeImpl(Message& msg) override
    {
        DataInfo::DataSeq data;
        auto writeIter = std::back_inserter(data);
        auto es =
            m_protStack.write(
                static_cast<const ProtocolMessage&>(msg),
                writeIter,
                data.max_size());
        if (es == comms::ErrorStatus::UpdateRequired) {
            auto updateIter = &data[0];
            es = m_protStack.update(
                static_cast<const ProtocolMessage&>(msg), 
                updateIter, 
                data.size());
        }

        if (es != comms::ErrorStatus::Success) {
            [[maybe_unused]] static constexpr bool Unexpected_write_update_failure = false;
            assert(Unexpected_write_update_failure); 
            return DataInfoPtr();
        }

        auto dataInfo = makeDataInfo();
        assert(dataInfo);

        dataInfo->m_timestamp = DataInfo::TimestampClock::now();
        dataInfo->m_data = std::move(data);
        dataInfo->m_extraProperties = getExtraInfoFromMessageProperties(msg);
        return dataInfo;
    }

    /// @brief Overriding implementation to Protocol::updateMessageImpl().
    virtual UpdateStatus updateMessageImpl(Message& msg) override
    {
        bool refreshed = msg.refreshMsg();

        assert(!msg.idAsString().isEmpty());
        do {
            std::vector<std::uint8_t> data;

            auto writeIter = std::back_inserter(data);
            auto es =
                m_protStack.write(
                    static_cast<const ProtocolMessage&>(msg),
                    writeIter,
                    data.max_size());
            if (es == comms::ErrorStatus::UpdateRequired) {
                auto updateIter = &data[0];
                es = m_protStack.update(
                    static_cast<const ProtocolMessage&>(msg),
                    updateIter, 
                    data.size());
            }

            if (es != comms::ErrorStatus::Success) {
                [[maybe_unused]] static constexpr bool Unexpected_write_update_failure = false;
                assert(Unexpected_write_update_failure); 
                break;
            }

            auto readMessageFunc =
                [&data](ProtocolMessage& msgToRead) -> bool
                {
                    typename ProtocolMessage::ReadIterator iter = nullptr;
                    if (!data.empty()) {
                        iter = &data[0];
                    }

                    auto esTmp = msgToRead.read(iter, data.size());
                    if (esTmp != comms::ErrorStatus::Success) {
                        return false;
                    }

                    return true;
                };

            std::unique_ptr<TransportMsg> transportMsgPtr(new TransportMsg());
            if (!readMessageFunc(*transportMsgPtr)) {
                [[maybe_unused]] static constexpr bool Unexpected_failure_to_read_transport_message = false;
                assert(Unexpected_failure_to_read_transport_message);        
                break;
            }

            std::unique_ptr<RawDataMsg> rawDataMsgPtr(new RawDataMsg());
            if (!readMessageFunc(*rawDataMsgPtr)) {
                [[maybe_unused]] static constexpr bool Unexpected_failure_to_read_raw_data = false;
                assert(Unexpected_failure_to_read_raw_data);                   
                break;
            }

            setTransportToMessageProperties(MessagePtr(transportMsgPtr.release()), msg);
            setRawDataToMessageProperties(MessagePtr(rawDataMsgPtr.release()), msg);

            auto extraProps = getExtraInfoFromMessageProperties(msg);
            bool extraInfoMsgIsForced = getForceExtraInfoExistenceFromMessageProperties(msg);
            if (extraProps.isEmpty() && (!extraInfoMsgIsForced)) {
                setExtraInfoMsgToMessageProperties(MessagePtr(), msg);
                break;
            }

            std::unique_ptr<ExtraInfoMsg> extraInfoMsgPtr(new ExtraInfoMsg());
            if (extraProps.isEmpty()) {
                setExtraInfoMsgToMessageProperties(MessagePtr(extraInfoMsgPtr.release()), msg);
                break;
            }

            auto jsonObj = QJsonObject::fromVariantMap(extraProps);
            QJsonDocument doc(jsonObj);

            auto& str = std::get<0>(extraInfoMsgPtr->fields());
            str.value() = doc.toJson().constData();
            setExtraInfoMsgToMessageProperties(
                MessagePtr(extraInfoMsgPtr.release()),
                msg);

        } while (false);

        if (refreshed) {
            return UpdateStatus::Changed;
        }

        return UpdateStatus::NoChange;
    }

    /// @brief Overriding implementation to Protocol::cloneMessageImpl().
    virtual MessagePtr cloneMessageImpl(const Message& msg) override
    {
        MessagePtr clonedMsg;
        unsigned idx = 0;
        while (true) {
            auto msgId = static_cast<const ProtocolMessage&>(msg).getId();
            clonedMsg = m_protStack.createMsg(msgId, idx);
            if (!clonedMsg) {
                break;
            }

            if (clonedMsg->assign(msg)) {
                break;
            }

            clonedMsg.reset();
            ++idx;
        }
        return clonedMsg;
    }

    /// @brief Overriding implementation to Protocol::createInvalidMessageImpl().
    virtual MessagePtr createInvalidMessageImpl() override
    {
        MessagePtr msg(new InvalidMsg());
        setNameToMessageProperties(*msg);
        return msg;
    }

    /// @brief Overriding implementation to Protocol::createRawDataMessageImpl().
    virtual MessagePtr createRawDataMessageImpl() override
    {
        return MessagePtr(new RawDataMsg());
    }

    /// @brief Overriding implementation to Protocol::createExtraInfoMessageImpl().
    virtual MessagePtr createExtraInfoMessageImpl() override
    {
        return MessagePtr(new ExtraInfoMsg());
    }

    /// @brief Overriding implementation to Protocol::createAllMessagesImpl().
    virtual MessagesList createAllMessagesImpl() override
    {
        return createAllMessagesInTuple<AllMessages>();
    }

    /// @brief Overriding implementation to Protocol::createMessageImpl().
    virtual MessagePtr createMessageImpl(const QString& idAsString, unsigned idx) override
    {
        return createMessageInternal(idAsString, idx, MsgIdTypeTag());
    }

    /// @brief Get access to embedded "protocol stack" object.
    ProtocolStack& protocolStack()
    {
        return m_protStack;
    }

    /// @brief Get access to embedded "protocol stack" object.
    const ProtocolStack& protocolStack() const
    {
        return m_protStack;
    }

    /// @brief Helper function to create message
    MessagePtr createMessage(MsgIdParamType id, unsigned idx = 0)
    {
        auto msgPtr = m_protStack.createMsg(id, idx);
        if (msgPtr) {
            setNameToMessageProperties(*msgPtr);
            updateMessage(*msgPtr);
        }
        return MessagePtr(std::move(msgPtr));
    }

    /// @brief Helper function allowing creation of all messages, types of which
    ///     provided in the template parameter.
    template <typename TMsgsTuple>
    MessagesList createAllMessagesInTuple()
    {
        using Tag = 
            typename std::conditional<
                std::is_void<MsgFactory>::value,
                CreateWithTupleIterationTag,
                HasMsgFactoryTag
            >::type;

        auto allMsgs = createAllMessagesInTupleInternal<TMsgsTuple>(Tag());
        for (auto& msgPtr : allMsgs) {
            setNameToMessageProperties(*msgPtr);
            setForceExtraInfoExistenceToMessageProperties(*msgPtr);
            updateMessage(*msgPtr);
        }    

        return allMsgs;    
    }

private:
    struct NumericIdTag {};
    struct OtherIdTag {};
    struct HasMsgFactoryTag{};
    struct HasStaticIdsTag{};
    struct CreateWithLoopIterationTag{};
    struct CreateWithTupleIterationTag{};

    typedef typename std::conditional<
        (std::is_enum<MsgIdType>::value || std::is_integral<MsgIdType>::value),
        NumericIdTag,
        OtherIdTag
    >::type MsgIdTypeTag;

    static_assert(std::is_same<MsgIdTypeTag, NumericIdTag>::value,
        "Non-numeric IDs are not supported properly yet.");

    class AllMsgsCreateHelper
    {
    public:
        AllMsgsCreateHelper(MessagesList& allMsgs)
          : m_allMsgs(allMsgs)
        {
        }

        template <typename TMsg>
        void operator()()
        {
            m_allMsgs.push_back(MessagePtr(new TMsg()));
        }

    private:
        MessagesList& m_allMsgs;
    };

    class MsgCreateHelper
    {
    public:
        MsgCreateHelper(const QString& id, unsigned idx, MessagePtr& msg)
          : m_id(id),
            m_reqIdx(idx),
            m_msg(msg)
        {
        }

        template <typename TMsg>
        void operator()()
        {
            if (m_msg) {
                return;
            }

            MessagePtr msgPtr(new TMsg());
            if (m_id != msgPtr->idAsString()) {
                return;
            }

            if (m_currIdx == m_reqIdx) {
                m_msg = std::move(msgPtr);
                return;
            }

            ++m_currIdx;
        }

    private:
        const QString& m_id;
        unsigned m_reqIdx;
        MessagePtr& m_msg;
        unsigned m_currIdx = 0;
    };

    MessagePtr createMessageInternal(const QString& idAsString, unsigned idx, NumericIdTag)
    {
        MessagePtr result;
        do {
            bool ok = false;
            int numId = idAsString.toInt(&ok, 10);
            if (!ok) {
                numId = idAsString.toInt(&ok, 16);
                if (!ok) {
                    break;
                }
            }

            result = createMessage(static_cast<MsgIdType>(numId), idx);
        } while (false);
        return result;
    }

    MessagePtr createMessageInternal(const QString& idAsString, unsigned idx, OtherIdTag)
    {
        MessagePtr result;
        comms::util::tupleForEachType<AllMessages>(MsgCreateHelper(name(), idAsString, idx, result));
        if (result) {
            updateMessage(*result);
        }
        return result;
    }

    template <typename TMsgsTuple>
    MessagesList createAllMessagesInTupleInternal(CreateWithTupleIterationTag)
    {
        MessagesList allMsgs;
        comms::util::tupleForEachType<TMsgsTuple>(AllMsgsCreateHelper(allMsgs));
        return allMsgs;
    }    

    template <typename TMsgsTuple>
    MessagesList createAllMessagesInTupleInternal(HasMsgFactoryTag)
    {
        static_assert(std::tuple_size<TMsgsTuple>::value > 0U, "At least one message is expected to be defined");
        using FirstType = typename std::tuple_element<0, TMsgsTuple>::type;
        using LastType = typename std::tuple_element<std::tuple_size<TMsgsTuple>::value - 1U, TMsgsTuple>::type;

        using Tag = 
            std::conditional_t<
                FirstType::hasStaticMsgId() && LastType::hasStaticMsgId(),
                HasStaticIdsTag,
                CreateWithLoopIterationTag
            >;

        return createAllMessagesInTupleInternal<TMsgsTuple>(Tag());
    }

    template <typename TMsgsTuple>
    MessagesList createAllMessagesInTupleInternal(CreateWithLoopIterationTag)
    {
        static_assert(std::tuple_size<TMsgsTuple>::value > 0U, "At least one message is expected to be defined");
        using FirstType = typename std::tuple_element<0, TMsgsTuple>::type;
        using LastType = typename std::tuple_element<std::tuple_size<TMsgsTuple>::value - 1U, TMsgsTuple>::type;
        auto firstId = static_cast<std::uintmax_t>(FirstType::doGetId());
        auto lastId = static_cast<std::uintmax_t>(LastType::doGetId());

        MessagesList allMsgs;
        MsgFactory factory;
        for (std::uintmax_t id = firstId; id <= lastId; ++id) {
            auto count = factory.msgCount(static_cast<MsgIdType>(id));
            for (auto idx = 0U; idx < count; ++idx) {
                auto msgPtr = factory.createMsg(static_cast<MsgIdType>(id), idx);
                if (msgPtr) {
                    allMsgs.push_back(std::move(msgPtr));
                }
            }
        }

        return allMsgs;
    }    

    template <typename TMsgsTuple>
    MessagesList createAllMessagesInTupleInternal(HasStaticIdsTag)
    {
        static_assert(std::tuple_size<TMsgsTuple>::value > 0U, "At least one message is expected to be defined");
        using FirstType = typename std::tuple_element<0, TMsgsTuple>::type;
        using LastType = typename std::tuple_element<std::tuple_size<TMsgsTuple>::value - 1U, TMsgsTuple>::type;
        static_assert(FirstType::hasStaticMsgId(), "Invalid displatch");
        static_assert(LastType::hasStaticMsgId(), "Invalid displatch");
        
        static const auto FirstId = FirstType::staticMsgId();
        static const auto LastId = LastType::staticMsgId();

        // When to sparse, use tuple iteration
        using Tag = 
            std::conditional_t<
                static_cast<std::size_t>(FirstId - LastId) <= (std::tuple_size<TMsgsTuple>::value * 5),
                CreateWithLoopIterationTag,
                CreateWithTupleIterationTag
            >;

        return createAllMessagesInTupleInternal<TMsgsTuple>(Tag());
    }

    ProtocolStack m_protStack;
    std::vector<std::uint8_t> m_data;
    std::vector<std::uint8_t> m_garbage;
};

}  // namespace cc_tools_qt



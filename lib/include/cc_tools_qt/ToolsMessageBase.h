//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsField.h"
#include "cc_tools_qt/ToolsFrameCommon.h"
#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsProtMsgInterface.h"
#include "cc_tools_qt/details/ToolsFieldCreator.h"

#include "comms/util/Tuple.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <vector>

namespace cc_tools_qt
{

/// @brief Helper class used to define protocol message class
///     in <b>CommsChampion Tools</b> plugin environment.
/// @tparam TBase Base class that this class is expected to inherit. Expected to be @ref cc_tools_qt::ToolsMessage or derivative.
/// @tparam TProtMsg Type of the protocol message defined using @b comms::MessageBase class.
/// @tparam TActualMsg Type of the actual message class inheriting from this one
/// @headerfile cc_tools_qt/ToolsMessageBase.h
template <typename TBase, template<typename...> class TProtMsg, typename TActualMsg>
class ToolsMessageBase : public TBase
{
    using Base = TBase;
    
public:
    /// @brief Data sequence type
    using DataSeq = typename TBase::DataSeq;

    using ProtInterface = typename TBase::ProtInterface;

    using ProtOptions = typename TBase::ProtOptions;

    /// @brief Protocol definition message type
    using ProtMsg = TProtMsg<ProtInterface, ProtOptions>;

    using FieldsList = typename Base::FieldsList;

    /// @brief Handler class
    // using Handler = typename CommsBase::Handler;

    /// @brief Default constructor
    ToolsMessageBase() = default;

    ToolsMessageBase(const ProtMsg& msg) : 
        m_msg(msg) 
    {
    }

    ToolsMessageBase(ProtMsg&& msg) : 
        m_msg(std::move(msg))
    {
    }

    /// @brief Copy Constructor
    ToolsMessageBase(const ToolsMessageBase& other) :
        m_msg(other.m_msg)
    {
    }

    /// @brief Move Constructor
    ToolsMessageBase(ToolsMessageBase&& other) : 
        m_msg(std::move(other.m_msg))
    {
    }

    /// @brief Destructor
    ~ToolsMessageBase() noexcept = default;

    /// @brief Copy assignment operator
    ToolsMessageBase& operator=(const ToolsMessageBase& other)
    {
        m_msg = other.m_msg;
        return *this;
    }

    /// @brief Move assignment operator
    ToolsMessageBase& operator=(ToolsMessageBase&& other)
    {
        m_msg = std::move(other.m_msg);
        return *this;
    }

    ProtMsg& msg()
    {
        return m_msg;
    }

    const ProtMsg& msg() const
    {
        return m_msg;
    }

protected:

    /// @brief Overriding polymorphic name retrieval functionality
    virtual const char* nameImpl() const override
    {
        using Tag = 
            std::conditional_t<
                ProtMsg::hasCustomName(),
                HasNameTag,
                NoNameTag
            >;

        return nameInternal(Tag());
    }

    /// @brief Overriding polymorphic refresh functionality.
    /// @details Invokes @b refresh() inherited from the
    ///     provided interface class
    virtual bool refreshMsgImpl() override
    {
        return m_msg.doRefresh();
    }

    /// @brief Overriding polymorphic retrieval of the numeric id
    virtual qlonglong numericIdImpl() const override
    {
        using Tag = 
            std::conditional_t<
                ProtMsg::hasStaticMsgId(),
                HasIdTag,
                NoIdTag
            >;

        return numericIdInternal(Tag());
    }

    /// @brief Overriding implementation to cc_tools_qt::ToolsMessage::resetImpl()
    virtual void resetImpl() override
    {
        auto& actObj = static_cast<TActualMsg&>(*this);
        actObj = TActualMsg();
    }    

    /// @brief Overriding polymorphic assignment
    virtual bool assignImpl(const cc_tools_qt::ToolsMessage& other) override
    {
        auto* castedOther = dynamic_cast<const TActualMsg*>(&other);
        if (castedOther == nullptr) {
            return false;
        }

        assert(other.idAsString() == Base::idAsString());
        auto& actObj = static_cast<TActualMsg&>(*this);
        actObj = *castedOther;
        return true;
    }    

    /// @brief Overriding polymorphic validity check
    /// @details Invokes @b valid() inherited from provided interface
    ///     class.
    virtual bool isValidImpl() const override
    {
        return m_msg.doValid();
    }    

    /// @brief Overriding polymorphic serialisation functionaly
    /// @details Invokes @b write() inherited from provided interface
    ///     class.
    virtual DataSeq encodeDataImpl() const override
    {
        DataSeq data;
        data.reserve(m_msg.doLength());
        auto iter = std::back_inserter(data);
        [[maybe_unused]] auto es = m_msg.doWrite(iter, data.max_size());
        assert(es == comms::ErrorStatus::Success);
        return data;
    }

    /// @brief Overriding polymorphic deserialisation functionaly
    /// @details Invokes @b read() inherited from provided interface
    ///     class.
    virtual bool decodeDataImpl(const DataSeq& data) override
    {
        auto iter = data.data();
        auto es = m_msg.doRead(iter, data.size());
        return es == comms::ErrorStatus::Success;
    }

    virtual typename Base::Ptr cloneImpl() const override
    {
        return typename Base::Ptr(new TActualMsg(static_cast<const TActualMsg&>(*this)));
    }

    virtual void assignProtMessageImpl(void* protMsg) override
    {
        auto* protMsgBase = reinterpret_cast<ProtInterface*>(protMsg);
        auto* actProtMsg = static_cast<ProtMsg*>(protMsgBase);
        m_msg = std::move(*actProtMsg);
    }

    virtual DataSeq encodeFramedImpl(ToolsFrame& frame) const override
    {
        auto& castedFrame = static_cast<ToolsFrameCommon<TBase>&>(frame);
        return castedFrame.writeProtMsg(m_msg);
    }

    virtual FieldsList transportFieldsImpl() override
    {
        FieldsList fields;
        using Tag = 
            std::conditional_t<
                ProtMsg::hasTransportFields(),
                HasTransportFields,
                NoTransportFields
            >;

        updateTransportFieldsInternal(fields, Tag());
        return fields;
    }    

    virtual FieldsList payloadFieldsImpl() override
    {
        FieldsList fields;
        fields.reserve(std::tuple_size<typename ProtMsg::AllFields>::value);
        comms::util::tupleForEach(m_msg.fields(), details::ToolsFieldCreator(fields));
        return fields;
    }

private:
    struct HasIdTag {};
    struct NoIdTag {};
    struct HasNameTag {};
    struct NoNameTag {};  
    struct HasTransportFields {};  
    struct NoTransportFields {};

    qlonglong numericIdInternal(HasIdTag) const
    {
        static const bool IsNumeric =
            std::is_enum<typename ProtMsg::MsgIdType>::value ||
            std::is_integral<typename ProtMsg::MsgIdType>::value;

        static_assert(IsNumeric, "Only numeric message IDs are supported");
        return static_cast<qlonglong>(m_msg.doGetId());
    }   

    qlonglong numericIdInternal(NoIdTag) const
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return static_cast<qlonglong>(0);
    }     

    const char* nameInternal(HasNameTag) const
    {
        static_assert(comms::isMessageBase<ProtMsg>(), "ProtMsg is expected to be proper message");
        static_assert(ProtMsg::hasCustomName(), "ProtMsg is expected to define message name");

        return m_msg.doName();
    }   

    const char* nameInternal(NoNameTag) const
    {
        assert(false); // Should not be called
        static const char* NoName = "NO-NAME";
        return NoName;
    }     

    void updateTransportFieldsInternal(FieldsList& fields, HasTransportFields)
    {
        fields.reserve(std::tuple_size<typename ProtMsg::TransportFields>::value);
        comms::util::tupleForEach(m_msg.transportFields(), details::ToolsFieldCreator(fields));
    }
    
    void updateTransportFieldsInternal([[maybe_unused]] FieldsList& fields, NoTransportFields)
    {
    }    

    ProtMsg m_msg;
};

}  // namespace cc_tools_qt



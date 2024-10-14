//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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

#include <functional>

#include "Api.h"
#include "Message.h"
#include "ToolsField.h"

namespace cc_tools_qt
{

/// @brief Generic message handler used by @b CommsChampion @b Tools
/// @details Provides polymorphic behavior via its protected functions, which
///     can be overriden in the derived class.
/// @headerfile cc_tools_qt/MessageHandler.h
class CC_API MessageHandler
{
public:
    /// @brief Destructor
    virtual ~MessageHandler() noexcept;

    /// @brief Handle the message.
    /// @details The function invokes virtual beginMsgHandlingImpl() at
    ///     the begining of handling, then for every field the message
    ///     contains, the addFieldImpl() virtuam member function is invoked.
    ///     At the end the endMsgHandlingImpl() virtual member function is
    ///     invoked.
    /// @tparam TMessage Message type
    /// @param[in] msg Reference to message object
    template <typename TMessage>
    void handle(TMessage& msg)
    {
        beginMsgHandlingImpl(msg);
        addExtraTransportFieldsInternal(msg);
        // TODO:
        // auto& fields = msg.fields();
        // comms::util::tupleForEach(
        //     fields,
        //     FieldsWrapperCreateHelper(
        //         [this](ToolsFieldPtr wrapper)
        //         {
        //             addFieldImpl(std::move(wrapper));
        //         }));
        endMsgHandlingImpl();
    }

protected:
    /// @brief Polymorphic report about starting message handling
    /// @param[in] msg Reference to message object.
    virtual void beginMsgHandlingImpl(Message& msg);

    /// @brief Polymorphic request to add handling of the extra transport field.
    /// @param [in] wrapper Pointer to field wrapper.
    virtual void addExtraTransportFieldImpl(ToolsFieldPtr wrapper);

    /// @brief Polymorphic request to add handling of the message field.
    /// @param [in] wrapper Pointer to field wrapper.
    virtual void addFieldImpl(ToolsFieldPtr wrapper);

    /// @brief Polymorphic report about ending message handling.
    virtual void endMsgHandlingImpl();

private:
    // TODO
    // class FieldsWrapperCreateHelper
    // {
    // public:
    //     typedef std::function <void (ToolsFieldPtr)> WrapperDispatchFunc;
    //     FieldsWrapperCreateHelper(WrapperDispatchFunc&& dispatchOp)
    //       : m_dispatchOp(std::move(dispatchOp))
    //     {
    //     }

    //     template <typename TField>
    //     void operator()(TField&& field)
    //     {
    //         auto wraper =
    //             details::FieldWrapperCreator::createWrapper(
    //                 std::forward<TField>(field));
    //         m_dispatchOp(std::move(wraper));
    //     }

    // private:
    //     WrapperDispatchFunc m_dispatchOp;
    // };

    struct HasExtraTransportFieldsTag {};
    struct NoExtraTransportFieldsTag {};

    template <typename TMessage>
    void addExtraTransportFieldsInternal(TMessage& msg)
    {
        static_cast<void>(msg);
        // TODO:

        // using MessageType = typename std::decay<decltype(msg)>::type;
        // using Tag =
        //     typename std::conditional<
        //         MessageType::hasTransportFields(),
        //         HasExtraTransportFieldsTag,
        //         NoExtraTransportFieldsTag
        //     >::type;

        // addExtraTransportFieldsInternal(msg, Tag());
    }

    template <typename TMessage>
    void addExtraTransportFieldsInternal(TMessage&, NoExtraTransportFieldsTag)
    {
        // do nothing
    }

    template <typename TMessage>
    void addExtraTransportFieldsInternal([[maybe_unused]] TMessage& msg, HasExtraTransportFieldsTag)
    {
        // TODO
        // auto& fields = msg.transportFields();
        // comms::util::tupleForEach(
        //     fields,
        //     FieldsWrapperCreateHelper(
        //         [this](ToolsFieldPtr wrapper)
        //         {
        //             addExtraTransportFieldImpl(std::move(wrapper));
        //         }));
    }

};

}  // namespace cc_tools_qt



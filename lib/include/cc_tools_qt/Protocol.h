//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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

#include <memory>
#include <cstdint>
#include <cstddef>
#include <list>

#include <QtCore/QMetaType>
#include <QtCore/QString>

#include "Api.h"
#include "Message.h"
#include "ErrorStatus.h"
#include "DataInfo.h"

namespace cc_tools_qt
{

/// @brief Main polymorphic interface class for protocols.
/// @details It is used by @b CommsChampion @b Tools to create and manipulate
///     protocol messages.
/// @headerfile cc_tools_qt/Protocol.h

class CC_API Protocol
{
public:
    /// @brief List of messages
    using MessagesList = std::list<MessagePtr>;

    /// @brief List of raw data buffers
    using DataInfosList = std::list<DataInfoPtr>;

    /// @brief Type used to contain raw bytes seqence
    using MsgDataSeq = Message::DataSeq ;

    /// @brief Status of message "update" operation.
    enum class UpdateStatus
    {
        NoChange, ///< The message contents haven't been changed
        Changed ///< The message contents have been changed
    };

    /// @brief Destructor
    virtual ~Protocol() noexcept;

    /// @brief Retrieve name of the protocol.
    /// @details Invokes @ref nameImpl().
    const QString& name() const;

    /// @brief Read the received data input.
    /// @details Invokes @ref readImpl().
    /// @param[in] dataInfo Received data information
    /// @param[in] final Final input indication, if @b true no more data is expected
    /// @return List of created messages
    MessagesList read(const DataInfo& dataInfo, bool final = false);

    /// @brief Serialse message.
    /// @details Invokes @ref writeImpl().
    /// @param[in] msg Reference to message object, passed by non-const reference
    ///     to allow update of the message properties.
    /// @return Serialised data.
    DataInfoPtr write(Message& msg);

    /// @brief Create all messages supported by the protocol.
    /// @details Invokes @ref createAllMessagesImpl().
    MessagesList createAllMessages();

    /// @brief Create message object given string representation of the message ID.
    /// @details Invokes @ref createMessageImpl().
    /// @param[in] idAsString String representation of the message ID.
    /// @param[in] idx Index of the message type within the range of message types
    ///     with the same ID.
    MessagePtr createMessage(const QString& idAsString, unsigned idx = 0);

    /// @brief Update (or refresh) message contents
    /// @details Invokes @ref updateMessageImpl().
    /// @return Status of the update.
    UpdateStatus updateMessage(Message& msg);

    /// @brief Clone the message object
    /// @details Invokes @ref cloneMessageImpl().
    /// @return Pointer to newly created message with the same contents
    MessagePtr cloneMessage(const Message& msg);

    /// @brief Create dummy message containing invalid input
    /// @details Invokes @ref createInvalidMessageImpl().
    MessagePtr createInvalidMessage(const MsgDataSeq& data);

    /// @brief Make the protocol aware about socket connection status
    /// @details Invokes @ref socketConnectionReportImpl().
    /// @param[in] connected Socket connection status.
    void socketConnectionReport(bool connected);   

    /// @brief Make the protocol aware that the message has been received from remote end
    /// @details Invokes @ref messageReceivedReportImpl().
    /// @param[in] msg Pointer to the message object
    void messageReceivedReport(MessagePtr msg);

    /// @brief Make the protocol aware that the message has been sent out to the remote end
    /// @details Invokes @ref messageSentReportImpl().
    /// @param[in] msg Pointer to the message object
    void messageSentReport(MessagePtr msg);

    /// @brief Apply inter-plugin configuration.
    /// @details Allows one plugin to influence the configuration of another.
    ///     This function will be called for all currently chosen plugins to override
    ///     current configuration. Invokes polymorphic @ref applyInterPluginConfigImpl().
    /// @param[in] props Properties map.
    void applyInterPluginConfig(const QVariantMap& props);      

    /// @brief Set debug output level
    /// @param[in] level Debug level. If @b 0, debug output is disabled
    void setDebugOutputLevel(unsigned level = 0U);     

    /// @brief Type of callback to report errors
    using ErrorReportCallback = std::function<void (const QString& msg)>;

    /// @brief Set callback to report errors
    /// @details The callback must have the same signature as @ref ErrorReportCallback
    template <typename TFunc>
    void setErrorReportCallback(TFunc&& func)
    {
        m_errorReportCallback = std::forward<TFunc>(func);
    }     

    /// @brief Type of callback to request message being sent initiated by the plugin itself
    using SendMessageRequestCallback = std::function<void (MessagePtr)>;

    /// @brief Set the callback to allow request of extra messages to be sent out
    /// @details The callback must have the same signature as @ref SendMessageRequestCallback
    template <typename TFunc>
    void setSendMessageRequestCallback(TFunc&& func)
    {
        m_sendMessageRequestCallback = std::forward<TFunc>(func);
    }

    /// @brief Callback to report inter-plugin configuration updates
    using InterPluginConfigReportCallback = std::function <void (const QVariantMap&)>;

    /// @brief Set callback to report inter-plugin configuration.
    /// @details The callback must have the same signature as @ref InterPluginConfigReportCallback.
    template <typename TFunc>
    void setInterPluginConfigReportCallback(TFunc&& func)
    {
        m_interPluginConfigReportCallback = std::forward<TFunc>(func);
    }    

protected:
    /// @brief Polymorphic protocol name retrieval.
    /// @details Invoked by name().
    virtual const QString& nameImpl() const = 0;

    /// @brief Polymorphic read functionality.
    /// @details Invoked by read().
    virtual MessagesList readImpl(const DataInfo& dataInfo, bool final) = 0;

    /// @brief Polymorphic write functionality.
    /// @details invoked by write().
    virtual DataInfoPtr writeImpl(Message& msg) = 0;

    /// @brief Polymorphic creation of all messages protocol supports.
    /// @details Invoked by createAllMessages().
    virtual MessagesList createAllMessagesImpl() = 0;

    /// @brief Polymorphic message creation functionality.
    /// @details Invoked by createMessage().
    virtual MessagePtr createMessageImpl(const QString& idAsString, unsigned idx) = 0;

    /// @brief Polymorphic message update (refresh) functionality.
    /// @details Invoked by updateMessage().
    virtual UpdateStatus updateMessageImpl(Message& msg) = 0;

    /// @brief Polymorphic message cloning functionality.
    /// @details Invoked by cloneMessage().
    virtual MessagePtr cloneMessageImpl(const Message& msg) = 0;

    /// @brief Polymorphic creation of invalid message representation.
    /// @details Invoked by createInvalidMessage().
    virtual MessagePtr createInvalidMessageImpl() = 0;

    /// @brief Polymorphic creation of message object representing raw data
    virtual MessagePtr createRawDataMessageImpl() = 0;

    /// @brief Polymorphic creation of message object representing extra info.
    virtual MessagePtr createExtraInfoMessageImpl() = 0;

    /// @brief Polymorphic processing of the socket connection report
    /// @details Empty function, does nothing
    /// @param[in] connected Socket connection status
    virtual void socketConnectionReportImpl(bool connected);    

    /// @brief Polymorphic processing of the message reception report
    /// @details Empty function, does nothing.
    /// @param[in] msg Pointer to the message object
    virtual void messageReceivedReportImpl(MessagePtr msg);

    /// @brief Make the protocol aware that the message has been sent out to the remote end
    /// @details Empty function, does nothing
    /// @param[in] msg Pointer to the message object
    virtual void messageSentReportImpl(MessagePtr msg);    

    /// @brief Polymorphic inter-plugin configuration application.
    /// @details Invoked by the applyInterPluginConfig().
    /// @param[in] props Properties map.
    virtual void applyInterPluginConfigImpl(const QVariantMap& props);      

    /// @brief Helper function to assign protocol name to message properties.
    /// @details Expected to be used by the derived class.
    void setNameToMessageProperties(Message& msg);

    /// @brief Report operation error.
    /// @details This function is expected to be invoked by the derived class,
    ///     when some error is detected. This function will invoke
    ///     callback set by @ref setErrorReportCallback().
    /// @param[in] str Error string.
    void reportError(const QString& str);  

    /// @brief Request a protocol message to be sent out.
    /// @details This function is expected to be invoked by the derived class,
    ///     when an extra message needs to be sent out. This function will invoke
    ///     callback set by @ref setSendMessageRequestCallback().
    /// @param[in] msg Pointer to the message object.
    void sendMessageRequest(MessagePtr msg);  

    /// @brief Report inter-plugin configuration.
    /// @details Sometimes configuration of one plugin may influence configuration of another.
    ///     Use this function to report inter-plugin configuration properties.
    ///     When invoked all other plugins are expected to get their respecitve 
    ///     @ref applyInterPluginConfig() functions invoked.
    /// @param[in] props Reported properties.
    void reportInterPluginConfig(const QVariantMap& props);      

    /// @brief Get current debug output level
    unsigned getDebugOutputLevel() const;    

    /// @brief Helper function to assign "tranport message" object as a property
    ///     of application message object.
    static void setTransportToMessageProperties(MessagePtr transportMsg, Message& msg);

    /// @brief Helper function to assign "raw data message" object as a property
    ///     of application message object.
    static void setRawDataToMessageProperties(MessagePtr rawDataMsg, Message& msg);

    /// @brief Helper function to assign "extra info message" object as a property
    ///     of application message object.
    static void setExtraInfoMsgToMessageProperties(MessagePtr extraInfoMsg, Message& msg);

    /// @brief Helper function to retrieve "extra info message" object from properties
    ///     of the application message object.
    static MessagePtr getExtraInfoMsgToMessageProperties(const Message& msg);

    /// @brief Helper function to retrieve "extra info" from message properties.
    static QVariantMap getExtraInfoFromMessageProperties(const Message& msg);

    /// @brief Helper function to set "extra info" to message properties.
    static void setExtraInfoToMessageProperties(const QVariantMap& extraInfo, Message& msg);

    /// @brief Helper function to merge existing "extra info" and provided one.
    static void mergeExtraInfoToMessageProperties(const QVariantMap& extraInfo, Message& msg);

    /// @brief Helper function to force "extra info" existence.
    static void setForceExtraInfoExistenceToMessageProperties(Message& msg);

    /// @brief Helper function to check whether "extra info" existence is force.
    static bool getForceExtraInfoExistenceFromMessageProperties(const Message& msg);

private:
    ErrorReportCallback m_errorReportCallback;
    SendMessageRequestCallback m_sendMessageRequestCallback;
    InterPluginConfigReportCallback m_interPluginConfigReportCallback;
    unsigned m_debugLevel = 0U;
};

/// @brief Pointer to @ref Protocol object.
using ProtocolPtr = std::shared_ptr<Protocol>;

}  // namespace cc_tools_qt

Q_DECLARE_METATYPE(cc_tools_qt::ProtocolPtr);

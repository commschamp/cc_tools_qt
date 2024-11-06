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

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsField.h"

#include "comms/ErrorStatus.h"

#include <QtCore/QObject>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

#include <cstdint>
#include <memory>
#include <vector>

namespace cc_tools_qt
{

class ToolsFrame;

/// @brief Main interface class used by <b>CommsChampion Tools</b>
///     to display and manipulate messages.
/// @headerfile cc_tools_qt/ToolsMessage.h
class CC_TOOLS_API ToolsMessage : public QObject
{
    using Base = QObject;
public:
    /// @brief Pointer to message object
    using Ptr = std::shared_ptr<ToolsMessage>;

    /// @brief Type for sequence of raw bytes
    using DataSeq = std::vector<std::uint8_t>;

    using FieldsList = std::vector<ToolsFieldPtr>;

    /// @brief Type of the message
    enum class Type {
        Invalid, ///< Invalid type
        Received, ///< Message has been received
        Sent, ///< Message has been sent
        NumOfValues ///< Number of available values
    };

    /// @brief Destructor
    /// @details virtual to allow polymorphic destruction
    virtual ~ToolsMessage() noexcept;

    /// @brief Get message name
    /// @details Invokes virtual nameImpl().
    const char* name() const;

    /// @brief Refresh message contents
    /// @details Needs to be invoked by the <b>CommsChampion Tools</b> when
    ///     some field get modified. Invokes refreshMsgImpl().
    /// @return true, if message contentes where refreshed (other fields changed).
    bool refreshMsg();

    /// @brief Get string representation of message ID.
    /// @details Invokes idAsStringImpl().
    QString idAsString() const;

    qlonglong numericId() const;

    /// @brief Reset message contents to default constructed values
    void reset();

    /// @brief Assign contents of other message to this ones
    /// @return true in case the messages are of the same type and the assignement
    ///     is successful, false othewise. Invokes assignImpl().
    bool assign(const ToolsMessage& other);

    /// @brief Check the message contents are valid.
    /// @details Invokes isValidImpl().
    bool isValid() const;

    /// @brief Encode (or serialise) message contents
    /// @details Invokes encodeDataImpl().
    DataSeq encodeData() const;

    /// @brief Decode (or deserialise) message contents
    /// @details Invokes decodeDataImpl().
    bool decodeData(const DataSeq& data);

    Ptr clone() const;

    void assignProtMessage(void* protMsg);

    DataSeq encodeFramed(ToolsFrame& frame) const;

    FieldsList transportFields();
    FieldsList payloadFields();

protected:

    /// @brief Polymorphic name retrieval functionality.
    /// @details Invoked by name().
    virtual const char* nameImpl() const = 0;

    /// @brief Polymorphic refresh functionality.
    /// @details Invoked by refresh().
    virtual bool refreshMsgImpl() = 0;

    /// @brief Polymophic functionality to get numeric message ID.
    /// @details Invoked by idAsString().
    virtual qlonglong numericIdImpl() const = 0;    

    /// @brief Polymophic functionality to get string representation of message ID.
    /// @details Invoked by idAsString().
    virtual QString idAsStringImpl() const;

    /// @brief Polymorphic reset functionality.
    /// @details Invoked by reset().
    virtual void resetImpl() = 0;

    /// @brief Polymophic assignment functionality.
    /// @details Invoked by assign().
    virtual bool assignImpl(const ToolsMessage& other) = 0;

    /// @brief Polymorphic validity check functionality.
    /// @details Invoked by isValid().
    virtual bool isValidImpl() const = 0;

    /// @brief Polymorphic serialisation functionality.
    /// @details Invoked by encodeData().
    virtual DataSeq encodeDataImpl() const = 0;

    /// @brief Polymorphic deserialisation functionality.
    /// @details Invoked by decodeData().
    virtual bool decodeDataImpl(const DataSeq& data) = 0;

    virtual Ptr cloneImpl() const = 0;

    virtual void assignProtMessageImpl(void* protMsg) = 0;

    virtual DataSeq encodeFramedImpl(ToolsFrame& frame) const = 0;

    virtual FieldsList transportFieldsImpl() = 0;
    virtual FieldsList payloadFieldsImpl() = 0;
};

/// @brief Smart pointer to @ref ToolsMessage
using ToolsMessagePtr = ToolsMessage::Ptr;

}  // namespace cc_tools_qt

Q_DECLARE_METATYPE(cc_tools_qt::ToolsMessagePtr);
Q_DECLARE_METATYPE(cc_tools_qt::ToolsMessage::DataSeq);
Q_DECLARE_METATYPE(cc_tools_qt::ToolsMessage::Type);

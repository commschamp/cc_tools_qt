//
// Copyright 2014 - 2023 (C). Alex Robenko. All rights reserved.
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

#include <cstdint>
#include <cstddef>
#include <vector>
#include <functional>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtCore/QString>
#include <QtCore/QList>
CC_ENABLE_WARNINGS()

#include "DataInfo.h"
#include "Api.h"

namespace cc_tools_qt
{

/// @brief Main polymorphic interface class for filters.
/// @details The filter can be used to implement encryption / decryption of
///     the data, and/or additional transport layer.
/// @headerfile cc_tools_qt/Filter.h
class CC_API Filter : public QObject
{
public:
    /// @brief Constructor
    Filter();

    /// @brief Destructor
    virtual ~Filter() noexcept;

    /// @brief Start filter's operation
    /// @details Prior to being started, the filter is not allowed to generate
    ///     any events. The function invokes virtual startImpl() which can
    ///     be overriden by the derived class.
    /// @return true in case the socket's operation has been successfully started,
    ///     false otherwise.
    bool start();

    /// @brief Stop filter's operation.
    /// @details After being stopped, the filter is not allowed to generate any
    ///     events. The function invokes virtual stopImpl(), which can be
    ///     overriden by the derived class
    void stop();

    /// @brief Process received data
    /// @details Process the data, received by I/O socket or other filter
    ///     down the chain.
    /// @param[in] dataPtr Incoming data from I/O socket or other filter
    ///     down the chain
    /// @return Data to forward to the protocol or to other filter up the
    ///     chain
    QList<DataInfoPtr> recvData(DataInfoPtr dataPtr);

    /// @brief Process outgoing data
    /// @details Process the data, generated by the protocol or other filter
    ///     up the chain.
    /// @param[in] dataPtr Outgoing data generated by the protocol or
    ///     other filter up the chain.
    /// @return Data to forward to the I/O socket or other filter down the
    ///     chain
    QList<DataInfoPtr> sendData(DataInfoPtr dataPtr);

    /// @brief Type of callback to report outgoing data.
    using DataToSendCallback = std::function<void (DataInfoPtr)>;

    /// @brief Set callback to report outgoing data.
    /// @details The filter is allowed to generate outgoing data independently
    ///     The provided callback is used to request its send.
    ///     The callback must have the same signature as @ref DataToSendCallback
    template <typename TFunc>
    void setDataToSendCallback(TFunc&& func)
    {
        m_dataToSendCallback = std::forward<TFunc>(func);
    }

    /// @brief Type of callback to report errors
    using ErrorReportCallback = std::function<void (const QString& msg)>;

    /// @brief Set callback to report errors
    /// @details The callback must have the same signature as @ref ErrorReportCallback
    template <typename TFunc>
    void setErrorReportCallback(TFunc&& func)
    {
        m_errorReportCallback = std::forward<TFunc>(func);
    }

protected:
    /// @brief Polymorphic start functionality implementation.
    /// @details Invoked by start() and default implementation does nothing.
    ///     It can be overrided by the derived class.
    /// @return true
    virtual bool startImpl();

    /// @brief Polymorphic stop functionality implementation.
    /// @details Invoked by stop() and default implementation does nothing.
    ///     It can be overridden by the derived class.
    virtual void stopImpl();

    /// @brief Polymorphic processing of incoming data.
    /// @details Invoked by recvData() and must be implemented in the derived
    ///     class
    virtual QList<DataInfoPtr> recvDataImpl(DataInfoPtr dataPtr) = 0;

    /// @brief Polymorphic processing of outgoing data.
    /// @details Invoked by sendData() and must be implemented in the derived
    ///     class
    virtual QList<DataInfoPtr> sendDataImpl(DataInfoPtr dataPtr) = 0;

    /// @brief Report new data to send generated by the filter itself.
    /// @details This function needs to be invoked by the derived class when
    ///     when it has new data to be sent over I/O link. This function
    ///     will invoke callback set by setDataToSendCallback().
    /// @param[in] dataPtr New data information.
    void reportDataToSend(DataInfoPtr dataPtr);

    /// @brief Report operation error.
    /// @details This function is expected to be invoked by the derived class,
    ///     when some error is detected. This function will invoke
    ///     callback set by setErrorReportCallback().
    /// @param[in] msg Error message.
    void reportError(const QString& msg);

private:
    DataToSendCallback m_dataToSendCallback;
    ErrorReportCallback m_errorReportCallback;
};

/// @brief Pointer to @ref Filter object.
using FilterPtr = std::shared_ptr<Filter>;

}  // namespace cc_tools_qt


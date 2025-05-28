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

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsDataInfo.h"
#include "cc_tools_qt/version.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

namespace cc_tools_qt
{

/// @brief Main polymorphic interface class for sockets.
/// @details It is used by @b CommsChampion @b Tools to drive low level
///     I/O operations.
/// @headerfile cc_tools_qt/ToolsSocket.h
class CC_TOOLS_API ToolsSocket : public QObject
{
    Q_OBJECT
public:
    /// @brief Available socket connection properties values.
    enum ConnectionProperty
    {
        ConnectionProperty_Autoconnect = 0x1, ///< Perfrom "connect" upon startup
        ConnectionProperty_NonDisconnectable = 0x2 ///< Do not allow socket disconnection
    };

    /// @brief Constructor
    ToolsSocket();

    /// @brief Destructor
    virtual ~ToolsSocket() noexcept;

    /// @brief Start socket's operation
    /// @details Prior to being started, the socket is not allowed to generate
    ///     any events. The function invokes virtual startImpl() which can
    ///     be overriden by the derived class.
    /// @return true in case the socket's operation has been successfully started,
    ///     false otherwise.
    bool start();

    /// @brief Stop socket's operation.
    /// @details After being stopped, the socket is not allowed to generate any
    ///     events. The function invokes virtual stopImpl(), which can be
    ///     overriden by the derived class
    void stop();

    /// @brief Report whether the socket has been properly started.
    bool isRunning() const;

    /// @brief Connect socket
    /// @details After the socket has been successfully started (see start()), the driving
    ///     application invokes this function as well. It can be used to open
    ///     the I/O device and/or connect to remote host (in case of network socket).
    ///     The function invokes virtual socketConnectImpl(), which can be overridden by
    ///     the derived class.
    /// @return true in case the operation is successful, false otherwise.
    bool socketConnect();

    /// @brief Disconnect socket
    /// @details This function is used to close the opened I/O device and/or
    ///     to disconnect from the remote host (in case of network socket).
    ///     The function invokes virtual socketDisconnectImpl(), which can be overridden by
    ///     the derived class.
    void socketDisconnect();

    /// @brief Check socket connected
    bool isSocketConnected() const;

    /// @brief Request socket to send data
    /// @details Invokes the sendDataImpl(), which must be implemented in
    ///     the derived class.
    /// @param[in] dataPtr Information about outging data
    void sendData(ToolsDataInfoPtr dataPtr);

    /// @brief Get properties describing socket connection right after plugins
    ///     have been loaded and applied.
    /// @details The returned value is used by the driving application to
    ///     properly present user interface. For example, whether the socket
    ///     needs to be auto-connected or the application should wait for
    ///     explicty user request.
    /// @return OR-ed values of @ref ConnectionProperty values.
    unsigned connectionProperties() const;

    /// @brief Apply inter-plugin configuration.
    /// @details Allows one plugin to influence the configuration of another.
    ///     This function will be called for all currently chosen plugins to override
    ///     current configuration. Invokes polymorphic @ref applyInterPluginConfigImpl().
    /// @param[in] props Properties map.
    void applyInterPluginConfig(const QVariantMap& props);       

    /// @brief Set debug output level
    /// @param[in] level Debug level. If @b 0, debug output is disabled
    void setDebugOutputLevel(unsigned level = 0U);

signals:
    /// @brief Signal used to report new data arrival
    /// @param[in] data New data object
    void sigDataReceivedReport(ToolsDataInfoPtr data);

    /// @brief Signal used to report error
    /// @param[in] msg Error message.    
    void sigErrorReport(const QString& msg);

    /// @brief Signal used to report socket connection status
    /// @param[in] connected Connection status
    void sigConnectionStatusReport(bool connected);

    /// @brief Signal to report inter-plugin configuration
    /// @param[in] props Configuration properties.
    void sigInterPluginConfigReport(const QVariantMap& props);

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

    /// @brief Polymorphic socket connection functionality implementation.
    /// @details Invoked by socketConnect() and default implementation does nothing.
    ///     It can be overridden by the derived class.
    /// @return true
    virtual bool socketConnectImpl();

    /// @brief Polymorphic socket disconnection functionality implementation.
    /// @details Invoked by socketDisconnect() and default implementation does nothing.
    ///     It can be overridden by the derived class.
    virtual void socketDisconnectImpl();

    /// @brief Polymorphic data send functionality implementation.
    /// @details Invoked by sendData(). It must be overridden and implemented in
    ///     the derived class.
    virtual void sendDataImpl(ToolsDataInfoPtr dataPtr) = 0;

    /// @brief Polymorphic connection properties functionality implementation.
    /// @details Invoked by connectionProperties(). In can be overriden by the
    ///     derived class.
    /// @return 0.
    virtual unsigned connectionPropertiesImpl() const;

    /// @brief Polymorphic inter-plugin configuration application.
    /// @details Invoked by the applyInterPluginConfig().
    /// @param[in] props Properties map.
    virtual void applyInterPluginConfigImpl(const QVariantMap& props);     

    /// @brief Get current timestamp
    static unsigned long long currTimestamp();

    /// @brief Get current debug output level
    unsigned getDebugOutputLevel() const;    

protected slots:
    /// @brief Report new data has been received.
    /// @details This function needs to be invoked by the derived class when
    ///     new data has been received from the I/O link. This function
    ///     will invoke callback set by setDataReceivedCallback().
    /// @param[in] dataPtr New data information.
    void reportDataReceived(ToolsDataInfoPtr dataPtr);

    /// @brief Report I/O operation error.
    /// @details This function is expected to be invoked by the derived class,
    ///     when some error is detected. This function will emit
    ///     @ref sigErrorReport() signal.
    /// @param[in] msg Error message.
    void reportError(const QString& msg);

    /// @brief Report socket disconnection.
    /// @details Sometimes socket may get unexpectedly disconnected. When such
    ///     situation is discovered, it should be reported to the running
    ////    application. This function is expected to be invoked by the
    ///     derived class and it will invoke callback set by setDisconnectedReportCallback().
    void reportDisconnected();

    /// @brief Report inter-plugin configuration.
    /// @details Sometimes configuration of one plugin may influence configuration of another.
    ///     Use this function to report inter-plugin configuration properties.
    ///     When invoked all other plugins are expected to get their respecitve 
    ///     @ref applyInterPluginConfig() functions invoked.@n
    ///     Emits @ref sigInterPluginConfigReport() signal.
    /// @param[in] props Reported properties.
    void reportInterPluginConfig(const QVariantMap& props);

private:
    struct InnerState;
    std::unique_ptr<InnerState> m_state;
};

/// @brief Pointer to @ref ToolsSocket object.
using ToolsSocketPtr = std::shared_ptr<ToolsSocket>;

}  // namespace cc_tools_qt


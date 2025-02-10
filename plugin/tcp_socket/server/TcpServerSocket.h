//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsSocket.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <list>

#ifdef CC_TOOLS_QT_DEFAULT_NETWORK_PORT
#define TCP_SERVER_DEFAULT_PORT CC_TOOLS_QT_DEFAULT_NETWORK_PORT    
#else
#define TCP_SERVER_DEFAULT_PORT 20000
#endif


namespace cc_tools_qt
{

namespace plugin
{

class TcpServerSocket : public cc_tools_qt::ToolsSocket
{
    Q_OBJECT
    using Base = cc_tools_qt::ToolsSocket;

public:
    using PortType = unsigned short;

    TcpServerSocket();
    ~TcpServerSocket() noexcept;

    void setPort(PortType value)
    {
        m_port = value;
    }

    PortType getPort() const
    {
        return m_port;
    }

signals:
    void sigConfigChanged();    

protected:
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(ToolsDataInfoPtr dataPtr) override;
    virtual unsigned connectionPropertiesImpl() const override;
    virtual void applyInterPluginConfigImpl(const QVariantMap& props) override;     

private slots:
    void newConnection();
    void connectionTerminated();
    void readFromSocket();
    void socketErrorOccurred(QAbstractSocket::SocketError err);
    void acceptErrorOccurred(QAbstractSocket::SocketError err);

private:
    static const PortType DefaultPort = TCP_SERVER_DEFAULT_PORT;
    PortType m_port = DefaultPort;
    std::list<QTcpSocket*> m_sockets;
    QTcpServer m_server;
};

} // namespace plugin

} // namespace cc_tools_qt

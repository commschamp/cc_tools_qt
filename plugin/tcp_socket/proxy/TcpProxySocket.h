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
#include <memory>
#include <utility>

#ifdef CC_TOOLS_QT_DEFAULT_NETWORK_PORT
#define TCP_PROXY_DEFAULT_PORT CC_TOOLS_QT_DEFAULT_NETWORK_PORT
#else
#define TCP_PROXY_DEFAULT_PORT 20000
#endif

namespace cc_tools_qt
{

namespace plugin
{

class TcpProxySocket : public cc_tools_qt::ToolsSocket
{
    Q_OBJECT
    using Base = cc_tools_qt::ToolsSocket;

public:
    typedef unsigned short PortType;

    TcpProxySocket();
    ~TcpProxySocket() noexcept;

    void setPort(PortType value)
    {
        m_port = value;
    }

    PortType getPort() const
    {
        return m_port;
    }

    void setRemoteHost(const QString& value)
    {
        m_remoteHost = value;
    }

    const QString& getRemoteHost() const
    {
        return m_remoteHost;
    }

    void setRemotePort(PortType value)
    {
        m_remotePort = value;
    }

    PortType getRemotePort() const
    {
        return m_remotePort;
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
    void clientConnectionTerminated();
    void readFromClientSocket();
    void socketErrorOccurred(QAbstractSocket::SocketError err);
    void acceptErrorOccurred(QAbstractSocket::SocketError err);
    void connectionSocketConnected();
    void connectionSocketDisconnected();
    void readFromConnectionSocket();

private:
    typedef QTcpSocket* ClientSocketPtr;
    typedef std::unique_ptr<QTcpSocket> ConnectionSocketPtr;
    typedef std::pair<ClientSocketPtr, ConnectionSocketPtr> ConnectedPair;
    typedef std::list<ConnectedPair> SocketsList;

    SocketsList::iterator findByClient(QTcpSocket* socket);
    SocketsList::iterator findByConnection(QTcpSocket* socket);
    void removeConnection(SocketsList::iterator iter);
    void performReadWrite(QTcpSocket& readFromSocket, QTcpSocket& writeToSocket);

    static const PortType DefaultPort = TCP_PROXY_DEFAULT_PORT;
    PortType m_port = DefaultPort;
    QString m_remoteHost;
    PortType m_remotePort = DefaultPort;

    QTcpServer m_server;
    SocketsList m_sockets;
};

} // namespace plugin

} // namespace cc_tools_qt

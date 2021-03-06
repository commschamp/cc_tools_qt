//
// Copyright 2016 - 2021 (C). Alex Robenko. All rights reserved.
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

#include <list>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtNetwork/QUdpSocket>
CC_ENABLE_WARNINGS()

#include "cc_tools_qt/Socket.h"


namespace cc_tools_qt
{

namespace plugin
{

namespace udp_socket
{

namespace client
{

class Socket : public QObject,
               public cc_tools_qt::Socket
{
    Q_OBJECT
    using Base = cc_tools_qt::Socket;

public:
    typedef unsigned short PortType;

    Socket();
    ~Socket() noexcept;

    void setHost(const QString& value)
    {
        m_host = value;
    }

    const QString& getHost() const
    {
        return m_host;
    }

    void setPort(PortType value)
    {
        m_port = value;
    }

    PortType getPort() const
    {
        return m_port;
    }

    void setLocalPort(PortType value)
    {
        m_localPort = value;
    }

    PortType getLocalPort() const
    {
        return m_localPort;
    }

    void setBroadcastPropName(const QString& value)
    {
        m_broadcastPropName = value;
    }

    const QString& getBroadcastPropName() const
    {
        return m_broadcastPropName;
    }

protected:
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(DataInfoPtr dataPtr) override;

private slots:
    void socketDisconnected();
    void readFromSocket();
    void readFromBroadcastSocket();
    void socketErrorOccurred(QAbstractSocket::SocketError err);

private:
    void readData(QUdpSocket& socket);
    bool bindSocket(QUdpSocket& socket);

    static const PortType DefaultPort = 20000;

    QString m_host;
    PortType m_port = DefaultPort;
    PortType m_localPort = 0;
    QString m_broadcastPropName;
    QUdpSocket m_socket;
    QUdpSocket m_broadcastSocket;
    bool m_running = false;
};

}  // namespace client

}  // namespace udp_socket

} // namespace plugin

} // namespace cc_tools_qt

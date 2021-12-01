//
// Copyright 2015 - 2021 (C). Alex Robenko. All rights reserved.
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
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
CC_ENABLE_WARNINGS()

#include "cc_tools_qt/Socket.h"


namespace cc_tools_qt
{

namespace plugin
{

namespace tcp_socket
{

namespace server
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

    void setPort(PortType value)
    {
        m_port = value;
    }

    PortType getPort() const
    {
        return m_port;
    }

protected:
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(DataInfoPtr dataPtr) override;
    virtual unsigned connectionPropertiesImpl() const override;

private slots:
    void newConnection();
    void connectionTerminated();
    void readFromSocket();
    void socketErrorOccurred(QAbstractSocket::SocketError err);
    void acceptErrorOccurred(QAbstractSocket::SocketError err);

private:
    static const PortType DefaultPort = 20000;
    PortType m_port = DefaultPort;
    std::list<QTcpSocket*> m_sockets;
    QTcpServer m_server;
};

}  // namespace server

}  // namespace tcp_socket

} // namespace plugin

} // namespace cc_tools_qt

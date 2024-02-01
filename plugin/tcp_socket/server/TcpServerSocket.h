//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include "cc_tools_qt/Socket.h"


namespace cc_tools_qt
{

namespace plugin
{

class TcpServerSocket : public QObject, public cc_tools_qt::Socket
{
    Q_OBJECT
    using Base = cc_tools_qt::Socket;

public:
    typedef unsigned short PortType;

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

} // namespace plugin

} // namespace cc_tools_qt

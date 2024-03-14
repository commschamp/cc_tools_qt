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

#include <cassert>

#include <QtCore/QtGlobal>
#include <QtNetwork/QHostAddress>

#include "TcpClientSocket.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace
{

const QString& fromProp()
{
    static const QString Str("tcp.from");
    return Str;
}

const QString& toProp()
{
    static const QString Str("tcp.to");
    return Str;
}

const QString& tcpHostProp()
{
    static const QString Str("tcp.host");
    return Str;
}

const QString& tcpPortProp()
{
    static const QString Str("tcp.port");
    return Str;
}

const QString& networkHostProp()
{
    static const QString Str("network.host");
    return Str;
}

const QString& networkPortProp()
{
    static const QString Str("network.port");
    return Str;
}

}  // namespace


TcpClientSocket::TcpClientSocket()
{
    connect(
        &m_socket, &QTcpSocket::disconnected,
        this, &TcpClientSocket::socketDisconnected);
    connect(
        &m_socket, &QTcpSocket::readyRead,
        this, &TcpClientSocket::readFromSocket);

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)        
    connect(
        &m_socket, &QTcpSocket::errorOccurred,
        this, &TcpClientSocket::socketErrorOccurred);
#else // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(
        &m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(socketErrorOccurred(QAbstractSocket::SocketError)));
#endif        
}

TcpClientSocket::~TcpClientSocket() noexcept
{
    m_socket.blockSignals(true);
}

bool TcpClientSocket::socketConnectImpl()
{
    if ((m_socket.state() == QTcpSocket::ConnectedState) ||
        (m_socket.state() == QTcpSocket::ConnectingState)) {
        [[maybe_unused]] static constexpr bool Already_connected = false;
        assert(Already_connected); 
        static const QString AlreadyConnectedError(
            tr("TCP/IP Client is already connected or trying to connect."));
        reportError(AlreadyConnectedError);
        return false;
    }

    auto port = getPortValue();
    if (port == 0) {
        static const QString PortError(
            tr("TCP/IP Client port is not configured."));
        reportError(PortError);
        return false;
    }

    auto host = getHostValue();
    if (host.isEmpty()) {
        host = QHostAddress(QHostAddress::LocalHost).toString();
    }

    m_socket.connectToHost(host, port);
    if (!m_socket.waitForConnected(1000)) {
        return false;
    }


    return true;
}

void TcpClientSocket::socketDisconnectImpl()
{
    m_socket.blockSignals(true);
    m_socket.flush();
    m_socket.disconnectFromHost();
    m_socket.close();
    m_socket.blockSignals(false);
}

void TcpClientSocket::sendDataImpl(DataInfoPtr dataPtr)
{
    assert(dataPtr);
    m_socket.write(
        reinterpret_cast<const char*>(&dataPtr->m_data[0]),
        static_cast<qint64>(dataPtr->m_data.size()));

    QString from =
        m_socket.localAddress().toString() + ':' +
                    QString("%1").arg(m_socket.localPort());
    QString to =
        m_socket.peerAddress().toString() + ':' +
                    QString("%1").arg(m_socket.peerPort());


    dataPtr->m_extraProperties.insert(fromProp(), from);
    dataPtr->m_extraProperties.insert(toProp(), to);

}

void TcpClientSocket::applyInterPluginConfigImpl(const QVariantMap& props)
{
    static const QString* Props[] = {
        &tcpHostProp(),
        &tcpPortProp(),
        &networkHostProp(),
        &networkPortProp()
    };

    m_overrideConfig.clear();
    for (auto* p : Props) {
        if (props.contains(*p)) {
            m_overrideConfig[*p] = props[*p];
        }
    }
}

void TcpClientSocket::socketDisconnected()
{
//    static const QString DisconnectedError(
//        tr("Connection to TCP/IP Server was disconnected."));
//    reportError(DisconnectedError);

    reportDisconnected();
}

void TcpClientSocket::readFromSocket()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket != nullptr);

    auto dataPtr = makeDataInfo();
    dataPtr->m_timestamp = DataInfo::TimestampClock::now();

    auto dataSize = socket->bytesAvailable();
    dataPtr->m_data.resize(static_cast<std::size_t>(dataSize));
    auto result =
        socket->read(reinterpret_cast<char*>(&dataPtr->m_data[0]), dataSize);
    if (result != dataSize) {
        dataPtr->m_data.resize(static_cast<std::size_t>(result));
    }

    QString from =
        m_socket.peerAddress().toString() + ':' +
                    QString("%1").arg(m_socket.peerPort());
    QString to =
        m_socket.localAddress().toString() + ':' +
                    QString("%1").arg(m_socket.localPort());

    dataPtr->m_extraProperties.insert(fromProp(), from);
    dataPtr->m_extraProperties.insert(toProp(), to);
    reportDataReceived(std::move(dataPtr));
}

void TcpClientSocket::socketErrorOccurred([[maybe_unused]] QAbstractSocket::SocketError err)
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket != nullptr);

    reportError(socket->errorString());

    if (socket->state() != QTcpSocket::ConnectedState) {
        reportDisconnected();
    }
}

QString TcpClientSocket::getHostValue() const
{
    auto hostVar = m_overrideConfig.value(tcpHostProp());
    if (hostVar.isValid()) {
        return hostVar.toString();
    }

    hostVar = m_overrideConfig.value(networkHostProp());
    if (hostVar.isValid()) {
        return hostVar.toString();
    }

    return m_host;    
}

TcpClientSocket::PortType TcpClientSocket::getPortValue() const
{
    auto portVar = m_overrideConfig.value(tcpPortProp());
    if (portVar.isValid()) {
        auto val = portVar.value<PortType>();
        if (val != 0) {
            return val;
        }
    }   

    portVar = m_overrideConfig.value(networkPortProp());
    if (portVar.isValid()) {
        auto val = portVar.value<PortType>();
        if (val != 0) {
            return val;
        }
    }   

    return m_port;     
}


} // namespace plugin

} // namespace cc_tools_qt

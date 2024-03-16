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

#include "TcpServerSocket.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace
{


const QString& tcpFromProp()
{
    static const QString Str("tcp.from");
    return Str;
}

const QString& tcpToProp()
{
    static const QString Str("tcp.to");
    return Str;
}

const QString& tcpPortProp()
{
    static const QString Str("tcp.port");
    return Str;
}

const QString& networkPortProp()
{
    static const QString Str("network.port");
    return Str;
}

}  // namespace

TcpServerSocket::TcpServerSocket()
{
    QObject::connect(
        &m_server, &QTcpServer::acceptError,
        this, &TcpServerSocket::acceptErrorOccurred);

    QObject::connect(
        &m_server, &QTcpServer::newConnection,
        this, &TcpServerSocket::newConnection);
}

TcpServerSocket::~TcpServerSocket() noexcept
{
    for (auto* socket : m_sockets) {
        socket->flush();
    }
}

bool TcpServerSocket::socketConnectImpl()
{
    if (m_server.isListening()) {
        [[maybe_unused]] static constexpr bool Already_listening = false;
        assert(Already_listening); 
        static const QString AlreadyListeningError(
            tr("Previous run of TCP/IP Server socket wasn't terminated properly."));
        reportError(AlreadyListeningError);
        return false;
    }

    if (!m_server.listen(QHostAddress::Any, m_port)) {
        static const QString FailedToListenError(
            tr("Failed to listen on specified TCP/IP port."));
        reportError(FailedToListenError);
        return false;
    }

    return true;
}

void TcpServerSocket::socketDisconnectImpl()
{
    m_server.close();
    assert(!m_server.isListening());
}

void TcpServerSocket::sendDataImpl(DataInfoPtr dataPtr)
{
    assert(dataPtr);

    QVariantList toList;

    for (auto* socketTmp : m_sockets) {
        auto* socket = qobject_cast<QTcpSocket*>(socketTmp);
        assert(socket != nullptr);
        socket->write(
            reinterpret_cast<const char*>(&dataPtr->m_data[0]),
            static_cast<qint64>(dataPtr->m_data.size()));

        QString to =
            socket->peerAddress().toString() + ':' +
                        QString("%1").arg(socket->peerPort());
        toList.append(to);
    }

    QString from =
        m_server.serverAddress().toString() + ':' +
                    QString("%1").arg(m_server.serverPort());

    dataPtr->m_extraProperties.insert(tcpFromProp(), from);
    dataPtr->m_extraProperties.insert(tcpToProp(), toList);
}

unsigned TcpServerSocket::connectionPropertiesImpl() const
{
    return ConnectionProperty_Autoconnect;
}

void TcpServerSocket::applyInterPluginConfigImpl(const QVariantMap& props)
{
    bool updated = false;

    static const QString* PortProps[] = {
        &networkPortProp(),
        &tcpPortProp(),
    };    

    for (auto* p : PortProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<int>())) {
            setPort(static_cast<PortType>(var.value<int>()));
            updated = true;
        }
    }

    if (updated) {
        emit sigConfigChanged();
    }
}

void TcpServerSocket::newConnection()
{
    auto *newConnSocket = m_server.nextPendingConnection();
    m_sockets.push_back(newConnSocket);
    connect(
        newConnSocket, &QTcpSocket::disconnected,
        newConnSocket, &TcpServerSocket::deleteLater);
    connect(
        newConnSocket, &QTcpSocket::disconnected,
        this, &TcpServerSocket::connectionTerminated);
    connect(
        newConnSocket, &QTcpSocket::readyRead,
        this, &TcpServerSocket::readFromSocket);

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)  
    connect(
        newConnSocket, &QTcpSocket::errorOccurred,
        this, &TcpServerSocket::socketErrorOccurred);      
#else // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)                
    connect(
        newConnSocket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(socketErrorOccurred(QAbstractSocket::SocketError)));
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)                
}

void TcpServerSocket::connectionTerminated()
{
    auto* socket = sender();
    auto iter = std::find(m_sockets.begin(), m_sockets.end(), socket);
    if (iter == m_sockets.end()) {
        [[maybe_unused]] static constexpr bool Must_have_found_socket = false;
        assert(Must_have_found_socket);          
        return;
    }

    m_sockets.erase(iter);
}

void TcpServerSocket::readFromSocket()
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
        socket->peerAddress().toString() + ':' +
                    QString("%1").arg(socket->peerPort());
    dataPtr->m_extraProperties.insert(tcpFromProp(), from);

    QString to =
        m_server.serverAddress().toString() + ':' +
                    QString("%1").arg(m_server.serverPort());
    dataPtr->m_extraProperties.insert(tcpToProp(), to);

    reportDataReceived(std::move(dataPtr));
}

void TcpServerSocket::socketErrorOccurred(QAbstractSocket::SocketError err)
{
    if (err == QAbstractSocket::RemoteHostClosedError) {
        // Ignore remote client disconnection
        return;
    }

    auto* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket != nullptr);

    reportError(socket->errorString());
}

void TcpServerSocket::acceptErrorOccurred([[maybe_unused]] QAbstractSocket::SocketError err)
{
    reportError(m_server.errorString());
    if (!m_server.isListening()) {
        reportDisconnected();
    }
}

} // namespace plugin

} // namespace cc_tools_qt

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

#include "TcpProxySocket.h"

#include <QtCore/QtGlobal>
#include <QtNetwork/QHostAddress>

#include <cassert>

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

const QString& tcpLocalPortProp()
{
    static const QString Str("tcp.local_port");
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

const QString& networkLocalPortProp()
{
    static const QString Str("network.local_port");
    return Str;
}

}  // namespace

TcpProxySocket::TcpProxySocket()
{
    QObject::connect(
        &m_server, &QTcpServer::newConnection,
        this, &TcpProxySocket::newConnection);

    QObject::connect(
        &m_server, &QTcpServer::acceptError,
        this, &TcpProxySocket::socketErrorOccurred);
}

TcpProxySocket::~TcpProxySocket() noexcept
{
    while (!m_sockets.empty()) {
        removeConnection(m_sockets.begin());
    }
}

bool TcpProxySocket::socketConnectImpl()
{
    if (m_server.isListening()) {
        [[maybe_unused]] static constexpr bool Already_listening = false;
        assert(Already_listening);
        static const QString AlreadyListeningError(
            tr("Previous run of TCP/IP Server socket wasn't terminated properly."));
        reportError(AlreadyListeningError);
        return false;
    }

    do {
        if (m_port != m_remotePort) {
            break;
        }

        if ((!m_remoteHost.isEmpty()) &&
            (m_remoteHost != "localhost") &&
            (m_remoteHost != "127.0.0.1")) {
            break;
        }

        static const QString PortsErrorError(
            tr("Cannot have the same port for local and remote endpoints."));
        reportError(PortsErrorError);
        return false;
    } while (false);

    if (!m_server.listen(QHostAddress::Any, m_port)) {
        static const QString FailedToListenError(
            tr("Failed to listen on specified TCP/IP port."));
        reportError(FailedToListenError);
        return false;
    }

    return true;
}

void TcpProxySocket::socketDisconnectImpl()
{
    m_server.close();
}

void TcpProxySocket::sendDataImpl(ToolsDataInfoPtr dataPtr)
{
    assert(dataPtr);
    QVariantList toList;
    for (auto& connectedPair : m_sockets) {
        assert(connectedPair.first != nullptr);
        assert(connectedPair.second);
        connectedPair.first->write(
            reinterpret_cast<const char*>(&dataPtr->m_data[0]),
            static_cast<qint64>(dataPtr->m_data.size()));
        connectedPair.second->write(
            reinterpret_cast<const char*>(&dataPtr->m_data[0]),
            static_cast<qint64>(dataPtr->m_data.size()));

        toList.append(
            connectedPair.first->peerAddress().toString() + ':' +
                        QString("%1").arg(connectedPair.first->peerPort()));

        toList.append(
            connectedPair.second->peerAddress().toString() + ':' +
                        QString("%1").arg(connectedPair.second->peerPort()));
    }
    QString from =
        m_server.serverAddress().toString() + ':' +
                    QString("%1").arg(m_server.serverPort());

    dataPtr->m_extraProperties.insert(tcpFromProp(), from);
    dataPtr->m_extraProperties.insert(tcpToProp(), toList);
}

unsigned TcpProxySocket::connectionPropertiesImpl() const
{
    return ConnectionProperty_Autoconnect;
}

void TcpProxySocket::applyInterPluginConfigImpl(const QVariantMap& props)
{
    bool updated = false;
    static const QString* HostProps[] = {
        &networkHostProp(),
        &tcpHostProp(),
    };

    for (auto* p : HostProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<QString>())) {
            setRemoteHost(var.value<QString>());
            updated = true;
        }
    }

    static const QString* PortProps[] = {
        &networkPortProp(),
        &tcpPortProp(),
    };

    for (auto* p : PortProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<int>())) {
            setRemotePort(static_cast<PortType>(var.value<int>()));
            updated = true;
        }
    }

    static const QString* ProxyPortProps[] = {
        &networkLocalPortProp(),
        &tcpLocalPortProp(),
    };

    for (auto* p : ProxyPortProps) {
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

void TcpProxySocket::newConnection()
{
    auto *newConnSocket = m_server.nextPendingConnection();
    connect(
        newConnSocket, &QTcpSocket::disconnected,
        this, &TcpProxySocket::clientConnectionTerminated);
    connect(
        newConnSocket, &QTcpSocket::errorOccurred,
        this, &TcpProxySocket::socketErrorOccurred);

    ConnectionSocketPtr connectionSocket(new QTcpSocket);
    connect(
        connectionSocket.get(), &QTcpSocket::connected,
        this, &TcpProxySocket::connectionSocketConnected);
    connect(
        connectionSocket.get(), &QTcpSocket::disconnected,
        this, &TcpProxySocket::connectionSocketDisconnected);
    connect(
        connectionSocket.get(), &QTcpSocket::readyRead,
        this, &TcpProxySocket::readFromConnectionSocket);

    connect(
        connectionSocket.get(), &QTcpSocket::errorOccurred,
        this, &TcpProxySocket::socketErrorOccurred);

    if (m_remoteHost.isEmpty()) {
        m_remoteHost = QHostAddress(QHostAddress::LocalHost).toString();
    }

    connectionSocket->connectToHost(m_remoteHost, m_remotePort);
    m_sockets.emplace_back(newConnSocket, std::move(connectionSocket));
}

void TcpProxySocket::clientConnectionTerminated()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == nullptr) {
        [[maybe_unused]] static constexpr bool Signal_from_unknown_object = false;
        assert(Signal_from_unknown_object);
        return;
    }

    auto iter = findByClient(socket);
    if (iter == m_sockets.end()) {
        return;
    }

    assert(iter->second);
    socket->blockSignals(true);
    iter->second->blockSignals(true);
    iter->second->flush();
    m_sockets.erase(iter);
    socket->deleteLater();
}

void TcpProxySocket::readFromClientSocket()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket != nullptr);

    auto iter = findByClient(socket);
    assert (iter != m_sockets.end());
    assert(iter->second);
    auto& connectionSocket = *(iter->second);

    performReadWrite(*socket, connectionSocket);
}

void TcpProxySocket::socketErrorOccurred(QAbstractSocket::SocketError err)
{
    if (err == QAbstractSocket::RemoteHostClosedError) {
        // Ignore remote client disconnection
        return;
    }

    auto* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket != nullptr);

    reportError(socket->errorString());
}

void TcpProxySocket::acceptErrorOccurred([[maybe_unused]] QAbstractSocket::SocketError err)
{
    reportError(m_server.errorString());
    if (!m_server.isListening()) {
        reportDisconnected();
    }
}

void TcpProxySocket::connectionSocketConnected()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == nullptr) {
        [[maybe_unused]] static constexpr bool Signal_from_unknown_object = false;
        assert(Signal_from_unknown_object);
        return;
    }

    auto iter = findByConnection(socket);
    assert(iter != m_sockets.end());
    assert(iter->first != nullptr);

    connect(
        iter->first, &QTcpSocket::readyRead,
        this, &TcpProxySocket::readFromClientSocket);

    if (0 < iter->first->bytesAvailable()) {
        assert(iter->second);
        performReadWrite(*iter->first, *iter->second);
    }
}

void TcpProxySocket::connectionSocketDisconnected()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == nullptr) {
        [[maybe_unused]] static constexpr bool Signal_from_unknown_object = false;
        assert(Signal_from_unknown_object);
        return;
    }

    auto iter = findByConnection(socket);
    if (iter == m_sockets.end()) {
        return;
    }

    assert(iter->first);
    iter->first->blockSignals(true);
    iter->first->flush();
    delete iter->first;

    assert(iter->second);
    iter->second->flush();
    iter->second.release()->deleteLater();
    m_sockets.erase(iter);
}

void TcpProxySocket::readFromConnectionSocket()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket != nullptr);

    auto iter = findByConnection(socket);
    assert (iter != m_sockets.end());
    assert(iter->first != nullptr);
    auto& clientSocket = *(iter->first);
    performReadWrite(*socket, clientSocket);
}

TcpProxySocket::SocketsList::iterator TcpProxySocket::findByClient(QTcpSocket* socket)
{
    return std::find_if(
        m_sockets.begin(), m_sockets.end(),
        [socket](const ConnectedPair& elem) -> bool
        {
            return elem.first == socket;
        });
}

TcpProxySocket::SocketsList::iterator TcpProxySocket::findByConnection(QTcpSocket* socket)
{
    return std::find_if(
        m_sockets.begin(), m_sockets.end(),
        [socket](const ConnectedPair& elem) -> bool
        {
            return elem.second.get() == socket;
        });

}

void TcpProxySocket::removeConnection(SocketsList::iterator iter)
{
    assert(iter != m_sockets.end());
    auto* clientSocket = iter->first;
    assert(clientSocket);

    ConnectionSocketPtr connectionSocket(std::move(iter->second));
    assert(connectionSocket);
    assert(!iter->second);

    m_sockets.erase(iter);

    clientSocket->blockSignals(true);
    connectionSocket->blockSignals(true);

    if (clientSocket->state() == QTcpSocket::ConnectedState) {
        clientSocket->flush();
        clientSocket->disconnectFromHost();
    }
    delete clientSocket;

    if (connectionSocket->state() == QTcpSocket::ConnectedState) {
        connectionSocket->flush();
        connectionSocket->disconnectFromHost();
    }
}

void TcpProxySocket::performReadWrite(QTcpSocket& readFromSocket, QTcpSocket& writeToSocket)
{
    if (readFromSocket.bytesAvailable() == 0) {
        return;
    }

    auto dataPtr = makeDataInfo();
    dataPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();

    auto dataSize = readFromSocket.bytesAvailable();
    dataPtr->m_data.resize(static_cast<std::size_t>(dataSize));
    auto result =
        readFromSocket.read(reinterpret_cast<char*>(&dataPtr->m_data[0]), dataSize);
    if (result != dataSize) {
        dataPtr->m_data.resize(static_cast<std::size_t>(result));
    }

    writeToSocket.write(
        reinterpret_cast<const char*>(&dataPtr->m_data[0]),
        static_cast<qint64>(dataPtr->m_data.size()));

    QString from =
        readFromSocket.peerAddress().toString() + ':' +
                    QString("%1").arg(readFromSocket.peerPort());
    QString to =
        writeToSocket.peerAddress().toString() + ':' +
                    QString("%1").arg(writeToSocket.peerPort());


    dataPtr->m_extraProperties.insert(tcpFromProp(), from);
    dataPtr->m_extraProperties.insert(tcpToProp(), to);

    reportDataReceived(std::move(dataPtr));
}

} // namespace plugin

} // namespace cc_tools_qt

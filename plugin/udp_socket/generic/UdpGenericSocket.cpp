//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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
#include <iostream>

#include <QtCore/QtGlobal>
#include <QtNetwork/QHostAddress>

#include "UdpGenericSocket.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace
{

const QString DefaultHost("127.0.0.1");

const QString& udpFromProp()
{
    static const QString Str("udp.from");
    return Str;
}

const QString& udpToProp()
{
    static const QString Str("udp.to");
    return Str;
}

const QString& udpBroadcastProp()
{
    static const QString Str("udp.broadcast");
    return Str;
}

const QString& udpBroadcastMaskProp()
{
    static const QString Str("udp.broadcast_mask");
    return Str;
}

const QString& udpHostProp()
{
    static const QString Str("udp.host");
    return Str;
}

const QString& udpPortProp()
{
    static const QString Str("udp.port");
    return Str;
}

const QString& udpLocalPortProp()
{
    static const QString Str("udp.local_port");
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

const QString& networkBroadcastProp()
{
    static const QString Str("network.broadcast");
    return Str;
}

const QString& networkBroadcastMaskProp()
{
    static const QString Str("network.broadcast_mask");
    return Str;
}

}  // namespace


UdpGenericSocket::UdpGenericSocket()
{
    connect(
        &m_socket, &QUdpSocket::disconnected,
        this, &UdpGenericSocket::socketDisconnected);
    connect(
        &m_socket, &QUdpSocket::readyRead,
        this, &UdpGenericSocket::readFromSocket);

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)        
    connect(
        &m_socket, &QUdpSocket::errorOccurred,
        this, &UdpGenericSocket::socketErrorOccurred);  

#else // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)        
    connect(
        &m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(socketErrorOccurred(QAbstractSocket::SocketError)));        
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)                
}

UdpGenericSocket::~UdpGenericSocket() noexcept
{
    m_socket.blockSignals(true);
}

bool UdpGenericSocket::socketConnectImpl()
{
    if ((!m_host.isEmpty()) && (m_port == 0)) {
        static const QString Error =
            "Remote port must be greater than 0.";
        reportError(Error);
        return false;
    }

    if ((m_host.isEmpty()) && (m_localPort == 0)) {
        static const QString Error =
            "If no remote host specified, provide a local port information.";
        reportError(Error);
        return false;
    }

    assert(!m_socket.isOpen());
    m_running = true;

    do {
        if (m_localPort == 0) {
            break;
        }

        if (!bindSocket(m_socket)) {
            reportError("Failed to bind UDP socket to port " + QString("%1").arg(m_localPort));
        }

    } while (false);

    do {
        if (m_host.isEmpty()) {
            break;
        }

        m_socket.connectToHost(m_host, m_port);
        if (!m_socket.waitForConnected()) {
            reportError("Failed to connect UDP socket to " + QString("%1:%2").arg(m_host).arg(m_port));
        }
    } while (false);
    return true;
}

void UdpGenericSocket::socketDisconnectImpl()
{
    m_socket.blockSignals(true);
    m_socket.close();
    m_running = false;
    m_socket.blockSignals(false);
}

void UdpGenericSocket::sendDataImpl(DataInfoPtr dataPtr)
{
    assert(dataPtr);
    QString from =
        m_socket.localAddress().toString() + ':' +
                    QString("%1").arg(m_socket.localPort());
    dataPtr->m_extraProperties.insert(udpFromProp(), from);

    do {
        bool broadcastRequested = false;
        static const QString* BroadcastProps[] = {
            &networkBroadcastProp(),
            &udpBroadcastProp(),
        };

        for (auto* s : BroadcastProps) {
            assert(s != nullptr);
            auto broadcastVar = dataPtr->m_extraProperties.value(*s);
            if (broadcastVar.isValid() && broadcastVar.canConvert<bool>()) {
                broadcastRequested = broadcastVar.value<bool>();
            }            
        }

        if ((!broadcastRequested) ||
            (!m_socket.isOpen()) ||
            (m_port == 0)) {
            break;
        }

        auto broadcastMask = m_broadcastMask;
        static const QString* BroadcastMaskProps[] = {
            &networkBroadcastMaskProp(),
            &udpBroadcastMaskProp(),
        };

        for (auto* s : BroadcastMaskProps) {
            assert(s != nullptr);
            auto broadcastMaskVar = dataPtr->m_extraProperties.value(*s);
            if (broadcastMaskVar.isValid() && broadcastMaskVar.canConvert<QString>()) {
                broadcastMask = broadcastMaskVar.toString();
            }            
        }

        std::size_t writtenCount = 0;
        while (writtenCount < dataPtr->m_data.size()) {
            auto remSize = static_cast<qint64>(dataPtr->m_data.size() - writtenCount);
            auto count =
                m_socket.writeDatagram(
                    reinterpret_cast<const char*>(&dataPtr->m_data[writtenCount]),
                    remSize,
                    QHostAddress(broadcastMask),
                    m_port);

            if (count < 0) {
                return;
            }

            writtenCount += static_cast<std::size_t>(count);
        }

        QString to =
            QHostAddress(QHostAddress::Broadcast).toString() + ':' +
                        QString("%1").arg(m_port);

        dataPtr->m_extraProperties.insert(udpToProp(), to);
        return;
    } while (false);

    if (!m_socket.isOpen()) {
        return;
    }

    std::size_t writtenCount = 0;
    while (writtenCount < dataPtr->m_data.size()) {
        auto remSize = static_cast<qint64>(dataPtr->m_data.size() - writtenCount);
        auto count =
            m_socket.write(
                reinterpret_cast<const char*>(&dataPtr->m_data[writtenCount]),
                remSize);
        if (count < 0) {
            return;
        }

        writtenCount += static_cast<std::size_t>(count);
    }

    QString to =
        m_socket.peerAddress().toString() + ':' +
                    QString("%1").arg(m_socket.peerPort());

    dataPtr->m_extraProperties.insert(udpToProp(), to);
}

void UdpGenericSocket::applyInterPluginConfigImpl(const QVariantMap& props)
{
    bool updated = false;
    static const QString* HostProps[] = {
        &networkHostProp(),
        &udpHostProp(),
    };

    for (auto* p : HostProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<QString>())) {
            setHost(var.value<QString>());
            updated = true;
        }
    }

    static const QString* PortProps[] = {
        &networkPortProp(),
        &udpPortProp(),
    };    

    for (auto* p : PortProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<int>())) {
            setPort(static_cast<PortType>(var.value<int>()));
            updated = true;
        }
    }

    static const QString* ProxyPortProps[] = {
        &networkLocalPortProp(),
        &udpLocalPortProp(),
    };    

    for (auto* p : ProxyPortProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<int>())) {
            setPort(static_cast<PortType>(var.value<int>()));
            updated = true;
        }
    }   

    static const QString* BroadcastMasksProps[] = {
        &networkBroadcastMaskProp(),
        &udpBroadcastMaskProp(),
    };    

    for (auto* p : BroadcastMasksProps) {
        auto var = props.value(*p);
        if ((var.isValid()) && (var.canConvert<QString>())) {
            setBroadcastMask(var.value<QString>());
            updated = true;
        }
    }      

    if (updated) {
        emit sigConfigChanged();
    }
}

void UdpGenericSocket::socketDisconnected()
{
    reportDisconnected();
}

void UdpGenericSocket::readFromSocket()
{
    while (m_socket.hasPendingDatagrams()) {
        QHostAddress senderAddress;
        quint16 senderPort;

        auto dataPtr = makeDataInfo();
        dataPtr->m_timestamp = DataInfo::TimestampClock::now();
        dataPtr->m_data.resize(static_cast<std::size_t>(m_socket.pendingDatagramSize()));
        m_socket.readDatagram(
            reinterpret_cast<char*>(&dataPtr->m_data[0]),
            static_cast<qint64>(dataPtr->m_data.size()),
            &senderAddress,
            &senderPort);

        QString from =
            senderAddress.toString() + ':' +
                        QString("%1").arg(senderPort);
        QString to =
            m_socket.localAddress().toString() + ':' +
                        QString("%1").arg(m_socket.localPort());

        dataPtr->m_extraProperties.insert(udpFromProp(), from);
        dataPtr->m_extraProperties.insert(udpToProp(), to);
        reportDataReceived(std::move(dataPtr));

        if (m_socket.state() != QUdpSocket::ConnectedState) {
            m_socket.connectToHost(senderAddress, senderPort);
            m_socket.waitForConnected();
            assert(m_socket.isOpen());
            assert(m_socket.state() == QUdpSocket::ConnectedState);
        }
    }
}

void UdpGenericSocket::socketErrorOccurred([[maybe_unused]] QAbstractSocket::SocketError err)
{
    std::cout << "ERROR: UDP Socket: " << m_socket.errorString().toStdString() << std::endl;
}

bool UdpGenericSocket::bindSocket(QUdpSocket& socket)
{
    if (!socket.bind(QHostAddress::AnyIPv4, m_localPort, QUdpSocket::ShareAddress)) {
        return false;
    }

    return socket.open(QUdpSocket::ReadWrite);
}

} // namespace plugin

} // namespace cc_tools_qt

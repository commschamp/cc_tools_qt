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

#include "UdpProxySocket.h"

#include <QtCore/QtGlobal>
#include <QtNetwork/QHostAddress>

#include <cassert>
#include <iostream>

namespace cc_tools_qt
{

namespace plugin
{

namespace
{

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

}  // namespace


UdpProxySocket::UdpProxySocket()
{
}

UdpProxySocket::~UdpProxySocket() noexcept
{
    if (m_listenSocket) {
        m_listenSocket->blockSignals(true);
    }
}

bool UdpProxySocket::socketConnectImpl()
{
    if (m_host.isEmpty()) {
        static const QString Error =
            "Remote host is not configured.";
        reportError(Error);
        return false;
    }

    if (m_port == 0) {
        static const QString Error =
            "Remote port must be greater than 0.";
        reportError(Error);
        return false;
    }

    if (m_localPort == 0) {
        static const QString Error =
            "Local port must be greater than 0.";
        reportError(Error);
        return false;
    }    

    if (!createListenSocket()) {
        m_listenSocket.reset();
        return false;
    }

    m_running = true;
    return true;
}

void UdpProxySocket::socketDisconnectImpl()
{
    if (m_listenSocket) {
        m_listenSocket->blockSignals(true);
    }

    if (m_remoteSocket) {
        m_remoteSocket->blockSignals(true);
    }

    m_listenSocket.reset();
    m_remoteSocket.reset();
    m_running = false;
}

void UdpProxySocket::sendDataImpl(ToolsDataInfoPtr dataPtr)
{
    if (!m_running) {
        return;
    }

    assert(dataPtr);
    assert(m_listenSocket);
    QString from =
        m_listenSocket->localAddress().toString() + ':' +
                    QString("%1").arg(m_listenSocket->localPort());
    dataPtr->m_extraProperties.insert(udpFromProp(), from);


    assert(m_listenSocket);
    if (!m_listenSocket->isOpen()) {
        return;
    }

    std::size_t writtenCount = 0;
    while (writtenCount < dataPtr->m_data.size()) {
        auto remSize = static_cast<qint64>(dataPtr->m_data.size() - writtenCount);
        auto count =
            m_listenSocket->write(
                reinterpret_cast<const char*>(&dataPtr->m_data[writtenCount]),
                remSize);

        if (count < 0) {
            return;
        }

        writtenCount += static_cast<std::size_t>(count);
    }

    QString to =
        m_listenSocket->peerAddress().toString() + ':' +
                    QString("%1").arg(m_listenSocket->peerPort());

    dataPtr->m_extraProperties.insert(udpToProp(), to);
}

void UdpProxySocket::applyInterPluginConfigImpl(const QVariantMap& props)
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
            setLocalPort(static_cast<PortType>(var.value<int>()));
            updated = true;
        }
    }    

    if (updated) {
        emit sigConfigChanged();
    }
}

void UdpProxySocket::listenSocketDisconnected()
{
    reportDisconnected();
    if (m_remoteSocket) {
        m_remoteSocket->blockSignals(true);
    }

    m_remoteSocket.reset();
    m_listenSocket->blockSignals(true);
    m_listenSocket.release()->deleteLater();
}

void UdpProxySocket::readFromListenSocket()
{
    createRemoteSocketIfNeeded();

    assert(m_listenSocket);
    while (m_listenSocket->hasPendingDatagrams()) {
        QHostAddress senderAddress;
        quint16 senderPort;

        auto dataPtr = makeDataInfo();
        dataPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();
        dataPtr->m_data.resize(static_cast<std::size_t>(m_listenSocket->pendingDatagramSize()));
        m_listenSocket->readDatagram(
            reinterpret_cast<char*>(&dataPtr->m_data[0]),
            static_cast<qint64>(dataPtr->m_data.size()),
            &senderAddress,
            &senderPort);

        if (!m_running) {
            continue;
        }

        if (m_listenSocket->state() != QUdpSocket::ConnectedState) {
            m_listenSocket->connectToHost(senderAddress, senderPort);
            if (!m_listenSocket->waitForConnected(100)) {
                std::cerr << "WARNING: cannot connect to the initiating UDP socket." << std::endl;
            }
        }

        QString from =
            senderAddress.toString() + ':' +
                        QString("%1").arg(senderPort);
        QString to =
            m_listenSocket->localAddress().toString() + ':' +
                        QString("%1").arg(m_listenSocket->localPort());

        if (m_remoteSocket) {
            m_remoteSocket->write(reinterpret_cast<const char*>(dataPtr->m_data.data()), static_cast<qint64>(dataPtr->m_data.size()));

            to = m_remoteSocket->peerAddress().toString() + ':' +
                        QString("%1").arg(m_remoteSocket->peerPort());
        }                          

        dataPtr->m_extraProperties.insert(udpFromProp(), from);
        dataPtr->m_extraProperties.insert(udpToProp(), to);
        reportDataReceived(std::move(dataPtr));
    }    
}

void UdpProxySocket::listenSocketErrorOccurred([[maybe_unused]] QAbstractSocket::SocketError err)
{
    assert(m_listenSocket);
    reportError(QString("Listen socket error: " + m_listenSocket->errorString()));
}

void UdpProxySocket::remoteSocketDisconnected()
{
    assert(m_remoteSocket);
    reportDisconnected();

    if (m_listenSocket) {
        m_remoteSocket->blockSignals(true);
    }
    
    m_listenSocket.reset();
    m_remoteSocket->blockSignals(true);
    m_remoteSocket.release()->deleteLater();
}

void UdpProxySocket::readFromRemoteSocket()
{
    assert(m_remoteSocket);
    while (m_remoteSocket->hasPendingDatagrams()) {
        QHostAddress senderAddress;
        quint16 senderPort;

        auto dataPtr = makeDataInfo();
        dataPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();
        dataPtr->m_data.resize(static_cast<std::size_t>(m_remoteSocket->pendingDatagramSize()));
        m_remoteSocket->readDatagram(
            reinterpret_cast<char*>(&dataPtr->m_data[0]),
            static_cast<qint64>(dataPtr->m_data.size()),
            &senderAddress,
            &senderPort);

        if (!m_running) {
            continue;
        }            

        QString from =
            senderAddress.toString() + ':' +
                        QString("%1").arg(senderPort);
        QString to =
            m_remoteSocket->localAddress().toString() + ':' +
                        QString("%1").arg(m_remoteSocket->localPort());

        if (m_listenSocket) {
            m_listenSocket->write(reinterpret_cast<const char*>(dataPtr->m_data.data()), static_cast<qint64>(dataPtr->m_data.size()));

            to = m_listenSocket->peerAddress().toString() + ':' +
                        QString("%1").arg(m_listenSocket->peerPort());
        }            

        dataPtr->m_extraProperties.insert(udpFromProp(), from);
        dataPtr->m_extraProperties.insert(udpToProp(), to);
        reportDataReceived(std::move(dataPtr));
    }    
}

void UdpProxySocket::remoteSocketErrorOccurred([[maybe_unused]] QAbstractSocket::SocketError err)
{
    assert(m_remoteSocket);
    reportError(QString("Remote connection socket error: " + m_remoteSocket->errorString()));
}

bool UdpProxySocket::createListenSocket()
{
    m_listenSocket = std::make_unique<QUdpSocket>();

    connect(
        m_listenSocket.get(), &QUdpSocket::disconnected,
        this, &UdpProxySocket::listenSocketDisconnected);
    connect(
        m_listenSocket.get(), &QUdpSocket::readyRead,
        this, &UdpProxySocket::readFromListenSocket);
    connect(
        m_listenSocket.get(), &QUdpSocket::errorOccurred,
        this, &UdpProxySocket::listenSocketErrorOccurred);  

    if (!m_listenSocket->bind(QHostAddress::AnyIPv4, m_localPort)) {
        reportError("Failed to bind UDP socket to port " + QString("%1").arg(m_localPort));
        return false;
    }

    if (!m_listenSocket->open(QUdpSocket::ReadWrite)) {
        reportError("Failed to open local UDP listening on port " + QString("%1").arg(m_localPort));
        return false;        
    }

    return true;
}

void UdpProxySocket::createRemoteSocketIfNeeded()
{
    if (m_remoteSocket) {
        return;
    }

    m_remoteSocket = std::make_unique<QUdpSocket>();

    connect(
        m_remoteSocket.get(), &QUdpSocket::disconnected,
        this, &UdpProxySocket::remoteSocketDisconnected);
    connect(
        m_remoteSocket.get(), &QUdpSocket::readyRead,
        this, &UdpProxySocket::readFromRemoteSocket);
    connect(
        m_remoteSocket.get(), &QUdpSocket::errorOccurred,
        this, &UdpProxySocket::remoteSocketErrorOccurred);  

    m_remoteSocket->connectToHost(m_host, m_port);
    if (!m_remoteSocket->waitForConnected(1000)) {
        reportError("Failed to connect to remote " + QString("%1:%2").arg(m_host).arg(m_port));
        m_remoteSocket.reset();
        return;        
    }
}

} // namespace plugin

} // namespace cc_tools_qt

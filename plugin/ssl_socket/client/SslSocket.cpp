//
// Copyright 2023 - 2023 (C). Alex Robenko. All rights reserved.
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

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslConfiguration>
CC_ENABLE_WARNINGS()

#include "SslSocket.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace ssl_socket
{

namespace client
{

namespace
{

const QString FromPropName("ssl.from");
const QString ToPropName("ssl.to");

}  // namespace


SslSocket::SslSocket()
{
    connect(
        &m_socket, SIGNAL(disconnected()),
        this, SLOT(socketDisconnected()));
    connect(
        &m_socket, SIGNAL(readyRead()),
        this, SLOT(readFromSocket()));
    connect(
        &m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(socketErrorOccurred(QAbstractSocket::SocketError)));
    connect(
        &m_socket, SIGNAL(sslErrors(const QList<QSslError>&)),
        this, SLOT(sslErrorsOccurred(const QList<QSslError>&)));        
}

SslSocket::~SslSocket() noexcept
{
    m_socket.blockSignals(true);
}

bool SslSocket::socketConnectImpl()
{
    if ((m_socket.state() == QSslSocket::ConnectedState) ||
        (m_socket.state() == QSslSocket::ConnectingState)) {
        static constexpr bool Already_connected = false;
        static_cast<void>(Already_connected);
        assert(Already_connected); 
        static const QString AlreadyConnectedError(
            tr("SSL Client is already connected or trying to connect."));
        reportError(AlreadyConnectedError);
        return false;
    }

    if (m_host.isEmpty()) {
        m_host = QHostAddress(QHostAddress::LocalHost).toString();
    }

    if ((!m_caDir.isEmpty()) || (!m_caFile.isEmpty())) {
        QSslConfiguration config;

        static const QString FailedToAddCaError(
            tr("Failed to add CA certificate(s) from "));        

        if (!m_caDir.isEmpty()) {
            if (!config.addCaCertificates(m_caDir, m_caDirFormat)) {
                reportError(FailedToAddCaError + m_caDir);
            }            
        }

        if (!m_caFile.isEmpty()) {
            if (!config.addCaCertificates(m_caFile, m_caFileFormat)) {
                reportError(FailedToAddCaError + m_caFile);
            }                
        }

        m_socket.setSslConfiguration(config);
    }

    m_socket.setPeerVerifyMode(m_verifyMode);
    m_socket.setPeerVerifyName(m_verifyName);
    m_socket.setProtocol(m_protocol);

    if (!m_certFile.isEmpty()) {
        m_socket.setLocalCertificate(m_certFile, m_certFormat);
    }

    if (!m_privKeyFile.isEmpty()) {
        m_socket.setPrivateKey(m_privKeyFile, m_privKeyAlg, m_privKeyFormat, m_privKeyPass.toUtf8());
    }

    m_socket.connectToHostEncrypted(m_host, m_port);
    if (!m_socket.waitForEncrypted(3000)) {
        return false;
    }

    return true;
}

void SslSocket::socketDisconnectImpl()
{
    m_socket.blockSignals(true);
    m_socket.flush();
    m_socket.disconnectFromHost();
    m_socket.close();
    m_socket.blockSignals(false);
}

void SslSocket::sendDataImpl(DataInfoPtr dataPtr)
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


    dataPtr->m_extraProperties.insert(FromPropName, from);
    dataPtr->m_extraProperties.insert(ToPropName, to);

}

void SslSocket::socketDisconnected()
{
    reportDisconnected();
}

void SslSocket::readFromSocket()
{
    auto* socket = qobject_cast<QSslSocket*>(sender());
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

    dataPtr->m_extraProperties.insert(FromPropName, from);
    dataPtr->m_extraProperties.insert(ToPropName, to);
    reportDataReceived(std::move(dataPtr));
}

void SslSocket::socketErrorOccurred(QAbstractSocket::SocketError err)
{
    static_cast<void>(err);
    auto* socket = qobject_cast<QSslSocket*>(sender());
    assert(socket != nullptr);

    reportError(socket->errorString());

    if (socket->state() != QSslSocket::ConnectedState) {
        reportDisconnected();
    }
}

void SslSocket::sslErrorsOccurred(const QList<QSslError>& errs)
{
    for (auto& e : errs) {
        reportError(e.errorString());
    }
}

}  // namespace client

}  // namespace ssl_socket

} // namespace plugin

} // namespace cc_tools_qt

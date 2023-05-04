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

#pragma once

#include <list>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtNetwork/QSslSocket>
CC_ENABLE_WARNINGS()

#include "cc_tools_qt/Socket.h"


namespace cc_tools_qt
{

namespace plugin
{

namespace ssl_socket
{

namespace client
{

class SslSocket : public QObject,
               public cc_tools_qt::Socket
{
    Q_OBJECT
    using Base = cc_tools_qt::Socket;

public:
    typedef unsigned short PortType;

    SslSocket();
    ~SslSocket() noexcept;

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

    void setCaFiles(const QString& value)
    {
        m_caFiles = value;
    }    

    const QString& getCaFiles() const
    {
        return m_caFiles;
    }

    void setCaFormat(QSsl::EncodingFormat value)
    {
        m_caFormat = value;
    }    

    QSsl::EncodingFormat getCaFormat() const
    {
        return m_caFormat;
    }

    void setVerifyMode(QSslSocket::PeerVerifyMode value)
    {
        m_verifyMode = value;
    }

    QSslSocket::PeerVerifyMode getVerifyMode() const
    {
        return m_verifyMode;
    }

    void setSslProtocol(QSsl::SslProtocol value)
    {
        m_protocol = value;
    }

    QSsl::SslProtocol getSslProtocol() const
    {
        return m_protocol;
    }

    void setCertFile(const QString& value)
    {
        m_certFile = value;
    }

    const QString& getCertFile() const
    {
        return m_certFile;
    }    

    void setCertFormat(QSsl::EncodingFormat value)
    {
        m_certFormat = value;
    }

    QSsl::EncodingFormat getCertFormat() const
    {
        return m_certFormat;
    }   

    void setPrivKeyFile(const QString& value)
    {
        m_privKeyFile = value;
    }     

    const QString& getPrivKeyFile() const
    {
        return m_privKeyFile;
    }

    void setPrivKeyAlg(QSsl::KeyAlgorithm value)
    {
        m_privKeyAlg = value;
    }    

    QSsl::KeyAlgorithm getPrivKeyAlg() const
    {
        return m_privKeyAlg;
    }

    void setPrivKeyFormat(QSsl::EncodingFormat value)
    {
        m_privKeyFormat = value;
    }    

    QSsl::EncodingFormat getPrivKeyFormat() const
    {
        return m_privKeyFormat;
    }

    void setPrivKeyPass(const QString& value)
    {
        m_privKeyPass = value;
    }
    
    const QString& getPrivKeyPass() const
    {
        return m_privKeyPass;
    }

protected:
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(DataInfoPtr dataPtr) override;

private slots:
    void socketDisconnected();
    void readFromSocket();
    void socketErrorOccurred(QAbstractSocket::SocketError err);
    void sslErrorsOccurred(const QList<QSslError>& errs);

private:
    static const PortType DefaultPort = 20000;
    QString m_host;
    PortType m_port = DefaultPort;
    QString m_caFiles;
    QSsl::EncodingFormat m_caFormat = QSsl::Pem;
    QSslSocket::PeerVerifyMode m_verifyMode = QSslSocket::VerifyNone;
    QSsl::SslProtocol m_protocol = QSsl::SecureProtocols;
    QString m_certFile;
    QSsl::EncodingFormat m_certFormat = QSsl::Pem;
    QString m_privKeyFile;
    QSsl::KeyAlgorithm m_privKeyAlg = QSsl::Rsa;
    QSsl::EncodingFormat m_privKeyFormat = QSsl::Pem;
    QString m_privKeyPass;
    QSslSocket m_socket;
};

}  // namespace client

}  // namespace ssl_socket

} // namespace plugin

} // namespace cc_tools_qt

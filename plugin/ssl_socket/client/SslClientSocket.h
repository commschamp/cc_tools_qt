//
// Copyright 2023 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsSocket.h"

#include <QtNetwork/QSslSocket>

#ifdef CC_TOOLS_QT_DEFAULT_NETWORK_PORT
#define SSL_CLIENT_DEFAULT_PORT CC_TOOLS_QT_DEFAULT_NETWORK_PORT    
#else
#define SSL_CLIENT_DEFAULT_PORT 20000
#endif

namespace cc_tools_qt
{

namespace plugin
{

class SslClientSocket : public cc_tools_qt::ToolsSocket
{
    Q_OBJECT
    using Base = cc_tools_qt::ToolsSocket;

public:
    typedef unsigned short PortType;

    SslClientSocket();
    ~SslClientSocket() noexcept;

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

    void setCaDir(const QString& value)
    {
        m_caDir = value;
    }    

    const QString& getCaDir() const
    {
        return m_caDir;
    }

    void setCaDirFormat(QSsl::EncodingFormat value)
    {
        m_caDirFormat = value;
    }    

    QSsl::EncodingFormat getCaDirFormat() const
    {
        return m_caDirFormat;
    }

    void setCaFile(const QString& value)
    {
        m_caFile = value;
    }    

    const QString& getCaFile() const
    {
        return m_caFile;
    }

    void setCaFileFormat(QSsl::EncodingFormat value)
    {
        m_caFileFormat = value;
    }    

    QSsl::EncodingFormat getCaFileFormat() const
    {
        return m_caFileFormat;
    }    

    void setVerifyMode(QSslSocket::PeerVerifyMode value)
    {
        m_verifyMode = value;
    }

    QSslSocket::PeerVerifyMode getVerifyMode() const
    {
        return m_verifyMode;
    }

    void setVerifyName(const QString& value)
    {
        m_verifyName = value;
    }

    const QString& getVerifyName() const
    {
        return m_verifyName;
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

signals:
    void sigConfigChanged();    

protected:
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(ToolsDataInfoPtr dataPtr) override;
    virtual void applyInterPluginConfigImpl(const QVariantMap& props) override;     

private slots:
    void socketDisconnected();
    void readFromSocket();
    void socketErrorOccurred(QAbstractSocket::SocketError err);
    void sslErrorsOccurred(const QList<QSslError>& errs);

private:
    static const PortType DefaultPort = SSL_CLIENT_DEFAULT_PORT;
    QString m_host;
    PortType m_port = DefaultPort;
    QString m_caDir;
    QSsl::EncodingFormat m_caDirFormat = QSsl::Pem;
    QString m_caFile;
    QSsl::EncodingFormat m_caFileFormat = QSsl::Pem;
    QSslSocket::PeerVerifyMode m_verifyMode = QSslSocket::VerifyNone;
    QString m_verifyName;
    QSsl::SslProtocol m_protocol = QSsl::SecureProtocols;
    QString m_certFile;
    QSsl::EncodingFormat m_certFormat = QSsl::Pem;
    QString m_privKeyFile;
    QSsl::KeyAlgorithm m_privKeyAlg = QSsl::Rsa;
    QSsl::EncodingFormat m_privKeyFormat = QSsl::Pem;
    QString m_privKeyPass;
    QSslSocket m_socket;
};

} // namespace plugin

} // namespace cc_tools_qt

//
// Copyright 2015 - 2023 (C). Alex Robenko. All rights reserved.
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

#include "SslSocketPlugin.h"

#include <memory>
#include <cassert>

#include "SslSocketConfigWidget.h"

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

const QString MainConfigKey("cc_ssl_client_socket");
const QString HostSubKey("host");
const QString PortSubKey("port");
const QString CaFileSubKey("ca");
const QString CaFormatSubKey("ca_format");
const QString VerifySubKey("verify");
const QString SslProtSubKey("prot");
const QString CertSubKey("cert");
const QString CertFormatSubKey("cert_format");
const QString PrivFileSubKey("priv");
const QString PrivAlgSubKey("priv_alg");
const QString PrivFormatSubKey("priv_format");


}  // namespace

SslSocketPlugin::SslSocketPlugin()
{
    pluginProperties()
        .setSocketCreateFunc(
            [this]()
            {
                createSocketIfNeeded();
                return m_socket;
            })
        .setConfigWidgetCreateFunc(
            [this]()
            {
                createSocketIfNeeded();
                return new SslSocketConfigWidget(*m_socket);
            });
}

SslSocketPlugin::~SslSocketPlugin() noexcept = default;

void SslSocketPlugin::getCurrentConfigImpl(QVariantMap& config)
{
    createSocketIfNeeded();

    QVariantMap subConfig;
    subConfig.insert(HostSubKey, m_socket->getHost());
    subConfig.insert(PortSubKey, m_socket->getPort());
    subConfig.insert(CaFileSubKey, m_socket->getCaFiles());
    subConfig.insert(CaFormatSubKey, static_cast<int>(m_socket->getCaFormat()));
    subConfig.insert(VerifySubKey, static_cast<int>(m_socket->getVerifyMode()));
    subConfig.insert(SslProtSubKey, static_cast<int>(m_socket->getSslProtocol()));
    subConfig.insert(CertSubKey, m_socket->getCertFile());
    subConfig.insert(CertFormatSubKey, static_cast<int>(m_socket->getCertFormat()));
    subConfig.insert(PrivFileSubKey, m_socket->getPrivKeyFile());
    subConfig.insert(PrivAlgSubKey, static_cast<int>(m_socket->getPrivKeyAlg()));
    subConfig.insert(PrivFormatSubKey, static_cast<int>(m_socket->getPrivKeyFormat()));
    config.insert(MainConfigKey, QVariant::fromValue(subConfig));
}

void SslSocketPlugin::reconfigureImpl(const QVariantMap& config)
{
    auto subConfigVar = config.value(MainConfigKey);
    if ((!subConfigVar.isValid()) || (!subConfigVar.canConvert<QVariantMap>())) {
        return;
    }

    createSocketIfNeeded();
    assert(m_socket);

    auto subConfig = subConfigVar.value<QVariantMap>();
    auto hostVar = subConfig.value(HostSubKey);
    if (hostVar.isValid() && hostVar.canConvert<QString>()) {
        auto host = hostVar.value<QString>();
        m_socket->setHost(host);
    }

    typedef SslSocket::PortType PortType;
    auto portVar = subConfig.value(PortSubKey);
    if (portVar.isValid() && portVar.canConvert<PortType>()) {
        auto port = portVar.value<PortType>();
        m_socket->setPort(port);
    }

    auto caVar = subConfig.value(CaFileSubKey);
    if (caVar.isValid() && caVar.canConvert<QString>()) {
        auto ca = caVar.value<QString>();
        m_socket->setCaFiles(ca);
    }    

    auto caFormatVar = subConfig.value(CaFormatSubKey);
    if (caFormatVar.isValid() && caFormatVar.canConvert<int>()) {
        auto caFormat = static_cast<QSsl::EncodingFormat>(caFormatVar.value<int>());
        m_socket->setCaFormat(caFormat);
    }

    auto verifyVar = subConfig.value(VerifySubKey);
    if (verifyVar.isValid() && verifyVar.canConvert<int>()) {
        auto verify = static_cast<QSslSocket::PeerVerifyMode>(verifyVar.value<int>());
        m_socket->setVerifyMode(verify);
    }  

    auto protVar = subConfig.value(SslProtSubKey);
    if (protVar.isValid() && protVar.canConvert<int>()) {
        auto prot = static_cast<QSsl::SslProtocol>(protVar.value<int>());
        m_socket->setSslProtocol(prot);
    }

    auto certVar = subConfig.value(CertSubKey);
    if (certVar.isValid() && certVar.canConvert<QString>()) {
        auto cert = certVar.value<QString>();
        m_socket->setCertFile(cert);
    }    

    auto certFormatVar = subConfig.value(CertFormatSubKey);
    if (certFormatVar.isValid() && certFormatVar.canConvert<int>()) {
        auto certFormat = static_cast<QSsl::EncodingFormat>(certFormatVar.value<int>());
        m_socket->setCertFormat(certFormat);
    }     

    auto privFileVar = subConfig.value(PrivFileSubKey);
    if (privFileVar.isValid() && privFileVar.canConvert<QString>()) {
        auto privFile = certVar.value<QString>();
        m_socket->setPrivKeyFile(privFile);
    } 

    auto prevAlgVar = subConfig.value(PrivAlgSubKey);
    if (prevAlgVar.isValid() && prevAlgVar.canConvert<int>()) {
        auto privAlg = static_cast<QSsl::KeyAlgorithm>(prevAlgVar.value<int>());
        m_socket->setPrivKeyAlg(privAlg);
    }  

    auto privFormatVar = subConfig.value(PrivFormatSubKey);
    if (privFormatVar.isValid() && privFormatVar.canConvert<int>()) {
        auto privFormat = static_cast<QSsl::EncodingFormat>(privFormatVar.value<int>());
        m_socket->setPrivKeyFormat(privFormat);
    } 
}

void SslSocketPlugin::createSocketIfNeeded()
{
    if (!m_socket) {
        m_socket.reset(new SslSocket());
    }
}

}  // namespace client

}  // namespace ssl_socket

}  // namespace plugin

}  // namespace cc_tools_qt



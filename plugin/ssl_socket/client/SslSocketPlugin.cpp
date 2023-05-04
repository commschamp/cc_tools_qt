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



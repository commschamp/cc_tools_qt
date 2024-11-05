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

#include "UdpProxySocketPlugin.h"

#include <memory>
#include <cassert>

#include "UdpProxySocketConfigWidget.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace
{

const QString MainConfigKey("cc_udp_proxy_socket");
const QString HostSubKey("remote_host");
const QString PortSubKey("remote_port");
const QString LocalPortSubKey("local_port");

}  // namespace

UdpProxySocketPlugin::UdpProxySocketPlugin() :
    Base(Type_Socket)
{
}

UdpProxySocketPlugin::~UdpProxySocketPlugin() noexcept = default;

void UdpProxySocketPlugin::getCurrentConfigImpl(QVariantMap& config)
{
    createSocketIfNeeded();

    QVariantMap subConfig;
    subConfig.insert(HostSubKey, m_socket->getHost());
    subConfig.insert(PortSubKey, m_socket->getPort());
    subConfig.insert(LocalPortSubKey, m_socket->getLocalPort());
    config.insert(MainConfigKey, QVariant::fromValue(subConfig));
}

void UdpProxySocketPlugin::reconfigureImpl(const QVariantMap& config)
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

    typedef UdpProxySocket::PortType PortType;
    auto portVar = subConfig.value(PortSubKey);
    if (portVar.isValid() && portVar.canConvert<PortType>()) {
        auto port = portVar.value<PortType>();
        m_socket->setPort(port);
    }

    auto localPortVar = subConfig.value(LocalPortSubKey);
    if (localPortVar.isValid() && localPortVar.canConvert<PortType>()) {
        auto port = localPortVar.value<PortType>();
        m_socket->setLocalPort(port);
    }
}

void UdpProxySocketPlugin::applyInterPluginConfigImpl(const QVariantMap& props)
{
    createSocketIfNeeded();
    m_socket->applyInterPluginConfig(props);
}

ToolsSocketPtr UdpProxySocketPlugin::createSocketImpl()
{
    createSocketIfNeeded();
    return m_socket;
}

QWidget* UdpProxySocketPlugin::createConfigurationWidgetImpl()
{
    createSocketIfNeeded();
    return new UdpProxySocketConfigWidget(*m_socket);
}

void UdpProxySocketPlugin::createSocketIfNeeded()
{
    if (!m_socket) {
        m_socket.reset(new UdpProxySocket());
    }
}

}  // namespace plugin

}  // namespace cc_tools_qt



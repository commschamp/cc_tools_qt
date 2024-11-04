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

#include "UdpGenericSocketPlugin.h"

#include <memory>
#include <cassert>

#include "UdpGenericSocketConfigWidget.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace
{

const QString MainConfigKey("cc_udp_socket");
const QString HostSubKey("host");
const QString PortSubKey("port");
const QString LocalPortSubKey("local_port");
const QString BroadcastMaskSubKey("broadcast_prop");

}  // namespace

UdpGenericSocketPlugin::UdpGenericSocketPlugin() :
    Base(Type_Socket)
{
}

UdpGenericSocketPlugin::~UdpGenericSocketPlugin() noexcept = default;

void UdpGenericSocketPlugin::getCurrentConfigImpl(QVariantMap& config)
{
    createSocketIfNeeded();

    QVariantMap subConfig;
    subConfig.insert(HostSubKey, m_socket->getHost());
    subConfig.insert(PortSubKey, m_socket->getPort());
    subConfig.insert(LocalPortSubKey, m_socket->getLocalPort());
    subConfig.insert(BroadcastMaskSubKey, m_socket->getBroadcastMask());
    config.insert(MainConfigKey, QVariant::fromValue(subConfig));
}

void UdpGenericSocketPlugin::reconfigureImpl(const QVariantMap& config)
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

    typedef UdpGenericSocket::PortType PortType;
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

    auto broadcastMaskVar = subConfig.value(BroadcastMaskSubKey);
    if (broadcastMaskVar.isValid() && broadcastMaskVar.canConvert<QString>()) {
        auto broadcastMask = broadcastMaskVar.value<QString>();
        m_socket->setBroadcastMask(broadcastMask);
    }
}

void UdpGenericSocketPlugin::applyInterPluginConfigImpl(const QVariantMap& props)
{
    createSocketIfNeeded();
    m_socket->applyInterPluginConfig(props);
}

SocketPtr UdpGenericSocketPlugin::createSocketImpl()
{
    createSocketIfNeeded();
    return m_socket;
}

QWidget* UdpGenericSocketPlugin::createConfigurationWidgetImpl()
{
    createSocketIfNeeded();
    return new UdpGenericSocketConfigWidget(*m_socket);
}

void UdpGenericSocketPlugin::createSocketIfNeeded()
{
    if (!m_socket) {
        m_socket.reset(new UdpGenericSocket());
    }
}

}  // namespace plugin

}  // namespace cc_tools_qt



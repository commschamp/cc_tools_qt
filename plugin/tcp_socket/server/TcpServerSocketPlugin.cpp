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

#include "TcpServerSocketPlugin.h"

#include "TcpServerSocket.h"
#include "TcpServerSocketConfigWidget.h"

#include <cassert>
#include <memory>

namespace cc_tools_qt
{

namespace plugin
{

namespace
{

const QString MainConfigKey("cc_tcp_server_socket");
const QString PortSubKey("port");

}  // namespace

TcpServerSocketPlugin::TcpServerSocketPlugin() :
    Base(Type_Socket)
{
}

TcpServerSocketPlugin::~TcpServerSocketPlugin() noexcept = default;

void TcpServerSocketPlugin::getCurrentConfigImpl(QVariantMap& config)
{
    createSocketIfNeeded();

    QVariantMap subConfig;
    subConfig.insert(PortSubKey, QVariant::fromValue(m_socket->getPort()));
    config.insert(MainConfigKey, QVariant::fromValue(subConfig));
}

void TcpServerSocketPlugin::reconfigureImpl(const QVariantMap& config)
{
    auto subConfigVar = config.value(MainConfigKey);
    if ((!subConfigVar.isValid()) || (!subConfigVar.canConvert<QVariantMap>())) {
        return;
    }

    typedef TcpServerSocket::PortType PortType;
    auto subConfig = subConfigVar.value<QVariantMap>();
    auto portVar = subConfig.value(PortSubKey);
    if ((!portVar.isValid()) || (!portVar.canConvert<PortType>())) {
        return;
    }

    auto port = portVar.value<PortType>();

    createSocketIfNeeded();

    m_socket->setPort(port);
}

void TcpServerSocketPlugin::applyInterPluginConfigImpl(const QVariantMap& props)
{
    createSocketIfNeeded();
    m_socket->applyInterPluginConfig(props);
}

ToolsSocketPtr TcpServerSocketPlugin::createSocketImpl()
{
    createSocketIfNeeded();
    return m_socket;
}

QWidget* TcpServerSocketPlugin::createConfigurationWidgetImpl()
{
    createSocketIfNeeded();
    return new TcpServerSocketConfigWidget(*m_socket);
}

void TcpServerSocketPlugin::createSocketIfNeeded()
{
    if (!m_socket) {
        m_socket.reset(new TcpServerSocket());
    }
}

}  // namespace plugin

}  // namespace cc_tools_qt



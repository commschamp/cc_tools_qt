//
// Copyright 2016 - 2021 (C). Alex Robenko. All rights reserved.
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

#include "SocketConfigWidget.h"

#include <limits>

namespace cc_tools_qt
{

namespace plugin
{

namespace udp_socket
{

namespace client
{

SocketConfigWidget::SocketConfigWidget(
    Socket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);

    m_ui.m_portSpinBox->setRange(
        0,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    m_ui.m_localPortSpinBox->setRange(
        0,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    m_ui.m_hostLineEdit->setText(m_socket.getHost());

    m_ui.m_portSpinBox->setValue(
        static_cast<int>(m_socket.getPort()));

    m_ui.m_localPortSpinBox->setValue(
        static_cast<int>(m_socket.getLocalPort()));

    m_ui.m_broadcastLineEdit->setText(m_socket.getBroadcastPropName());

    connect(
        m_ui.m_hostLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(hostValueChanged(const QString&)));

    connect(
        m_ui.m_portSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(portValueChanged(int)));

    connect(
        m_ui.m_localPortSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(localPortValueChanged(int)));

    connect(
        m_ui.m_broadcastLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(broadcastValueChanged(const QString&)));

}

SocketConfigWidget::~SocketConfigWidget() noexcept = default;

void SocketConfigWidget::hostValueChanged(const QString& value)
{
    m_socket.setHost(value);
}

void SocketConfigWidget::portValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

void SocketConfigWidget::localPortValueChanged(int value)
{
    m_socket.setLocalPort(static_cast<PortType>(value));
}

void SocketConfigWidget::broadcastValueChanged(const QString& value)
{
    m_socket.setBroadcastPropName(value);
}

}  // namespace client

}  // namespace udp_socket

}  // namespace plugin

}  // namespace cc_tools_qt



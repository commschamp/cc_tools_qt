//
// Copyright 2016 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "UdpGenericSocketConfigWidget.h"

#include <limits>

namespace cc_tools_qt
{

namespace plugin
{

UdpGenericSocketConfigWidget::UdpGenericSocketConfigWidget(
    UdpGenericSocket& socket,
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

    refresh();

    connect(
        &socket, &UdpGenericSocket::sigConfigChanged,
        this, &UdpGenericSocketConfigWidget::refresh);     

    connect(
        m_ui.m_hostLineEdit, &QLineEdit::textChanged,
        this, &UdpGenericSocketConfigWidget::hostValueChanged);

    connect(
        m_ui.m_portSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &UdpGenericSocketConfigWidget::portValueChanged);

    connect(
        m_ui.m_localPortSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &UdpGenericSocketConfigWidget::localPortValueChanged);

    connect(
        m_ui.m_broadcastMaskLineEdit, &QLineEdit::textChanged,
        this, &UdpGenericSocketConfigWidget::broadcastMaskValueChanged);

}

UdpGenericSocketConfigWidget::~UdpGenericSocketConfigWidget() noexcept = default;

void UdpGenericSocketConfigWidget::refresh()
{
    m_ui.m_hostLineEdit->setText(m_socket.getHost());

    m_ui.m_portSpinBox->setValue(
        static_cast<int>(m_socket.getPort()));

    m_ui.m_localPortSpinBox->setValue(
        static_cast<int>(m_socket.getLocalPort()));

    m_ui.m_broadcastMaskLineEdit->setText(m_socket.getBroadcastMask());
}

void UdpGenericSocketConfigWidget::hostValueChanged(const QString& value)
{
    m_socket.setHost(value);
}

void UdpGenericSocketConfigWidget::portValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

void UdpGenericSocketConfigWidget::localPortValueChanged(int value)
{
    m_socket.setLocalPort(static_cast<PortType>(value));
}

void UdpGenericSocketConfigWidget::broadcastMaskValueChanged(const QString& value)
{
    m_socket.setBroadcastMask(value);
}

}  // namespace plugin

}  // namespace cc_tools_qt



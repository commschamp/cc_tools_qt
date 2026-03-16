//
// Copyright 2016 - 2026 (C). Alex Robenko. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later
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

#include <algorithm>
#include <cassert>
#include <iterator>
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

    m_ui.m_interfaceComboBox->addItem(QString());
    m_networkInterfaces = QNetworkInterface::allInterfaces();
    for (auto& i : m_networkInterfaces) {
        m_ui.m_interfaceComboBox->addItem(i.name());
    }

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

    connect(
        m_ui.m_interfaceComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &UdpGenericSocketConfigWidget::currentInterfaceValueChanged);
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

    int idx = 0;
    do {
        auto& interface = m_socket.getInterface();
        if (interface.isEmpty()) {
            break;
        }

        auto iter = std::find_if(
            m_networkInterfaces.begin(), m_networkInterfaces.end(),
            [&interface](auto& netInterface)
            {
                return netInterface.name() == interface;
            });

        if (iter == m_networkInterfaces.end()) {
            m_socket.setInterface(QString());
            break;
        }

        auto dist = static_cast<int>(std::distance(m_networkInterfaces.begin(), iter));
        idx = dist + 1;
    } while (false);

    m_ui.m_interfaceComboBox->setCurrentIndex(idx);
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

void UdpGenericSocketConfigWidget::currentInterfaceValueChanged(int idx)
{
    if (idx == 0) {
        m_socket.setInterface(QString());
        return;
    }

    assert(idx <= m_networkInterfaces.size());
    m_socket.setInterface(m_networkInterfaces[idx - 1].name());
}

}  // namespace plugin

}  // namespace cc_tools_qt


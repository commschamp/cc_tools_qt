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

#include "TcpServerSocketConfigWidget.h"

#include <limits>

namespace cc_tools_qt
{

namespace plugin
{

TcpServerSocketConfigWidget::TcpServerSocketConfigWidget(
    TcpServerSocket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);

    m_ui.m_portSpinBox->setRange(
        1,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    refresh();

    connect(
        &socket, &TcpServerSocket::sigConfigChanged,
        this, &TcpServerSocketConfigWidget::refresh);

    connect(
        m_ui.m_portSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &TcpServerSocketConfigWidget::portValueChanged);
}

TcpServerSocketConfigWidget::~TcpServerSocketConfigWidget() noexcept = default;

void TcpServerSocketConfigWidget::refresh()
{
    m_ui.m_portSpinBox->setValue(
        static_cast<int>(m_socket.getPort()));
}

void TcpServerSocketConfigWidget::portValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

}  // namespace plugin

}  // namespace cc_tools_qt


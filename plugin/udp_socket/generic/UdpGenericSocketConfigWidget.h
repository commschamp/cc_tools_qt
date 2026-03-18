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

#pragma once

#include "ui_UdpGenericSocketConfigWidget.h"

#include "UdpGenericSocket.h"

#include <QtCore/QList>
#include <QtNetwork/QNetworkInterface>
#include <QtWidgets/QWidget>

namespace cc_tools_qt
{

namespace plugin
{

class UdpGenericSocketConfigWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    using PortType = UdpGenericSocket::PortType;

    explicit UdpGenericSocketConfigWidget(
        UdpGenericSocket& socket,
        QWidget* parentObj = nullptr);

    ~UdpGenericSocketConfigWidget() noexcept;

private slots:
    void refresh();
    void hostValueChanged(const QString& value);
    void portValueChanged(int value);
    void localPortValueChanged(int value);
    void broadcastMaskValueChanged(const QString& value);
    void currentInterfaceValueChanged(int idx);

private:
    using NetworkInterfacesList = QList<QNetworkInterface>;

    UdpGenericSocket& m_socket;
    Ui::UdpGenericSocketConfigWidget m_ui;
    NetworkInterfacesList m_networkInterfaces;
};

}  // namespace plugin

}  // namespace cc_tools_qt


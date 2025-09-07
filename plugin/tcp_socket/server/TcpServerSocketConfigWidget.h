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

#pragma once

#include "ui_TcpServerSocketConfigWidget.h"

#include "TcpServerSocket.h"

#include <QtWidgets/QWidget>

namespace cc_tools_qt
{

namespace plugin
{

class TcpServerSocketConfigWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Base;
public:
    typedef TcpServerSocket::PortType PortType;

    explicit TcpServerSocketConfigWidget(
        TcpServerSocket& socket,
        QWidget* parentObj = nullptr);

    ~TcpServerSocketConfigWidget() noexcept;

private slots:
    void refresh();
    void portValueChanged(int value);

private:
    TcpServerSocket& m_socket;
    Ui::TcpServerSocketConfigWidget m_ui;
};

}  // namespace plugin

}  // namespace cc_tools_qt


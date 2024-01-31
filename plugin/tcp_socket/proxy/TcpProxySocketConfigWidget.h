//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtWidgets/QWidget>
#include "ui_TcpProxySocketConfigWidget.h"
CC_ENABLE_WARNINGS()

#include "TcpProxySocket.h"

namespace cc_tools_qt
{

namespace plugin
{

class TcpProxySocketConfigWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Base;
public:
    typedef TcpProxySocket::PortType PortType;

    explicit TcpProxySocketConfigWidget(
        TcpProxySocket& socket,
        QWidget* parentObj = nullptr);

    ~TcpProxySocketConfigWidget() noexcept;

private slots:
    void localPortValueChanged(int value);
    void remoteHostValueChanged(const QString& value);
    void remotePortValueChanged(int value);

private:
    TcpProxySocket& m_socket;
    Ui::TcpProxySocketConfigWidget m_ui;
};

}  // namespace plugin

}  // namespace cc_tools_qt



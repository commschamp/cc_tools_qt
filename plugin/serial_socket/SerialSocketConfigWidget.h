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

#include <QtWidgets/QWidget>

#include "SerialSocket.h"
#include "ui_SerialSocketConfigWidget.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace serial_socket
{

class SerialSocketConfigWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Base;
public:
    explicit SerialSocketConfigWidget(
        SerialSocket& socket,
        QWidget* parentObj = nullptr);

    ~SerialSocketConfigWidget() noexcept;

private slots:
    void nameChanged(const QString& value);
    void deviceChanged(int idx);
    void deviceRefreshClicked(bool checked);
    void deviceEditClicked(bool checked);
    
    void baudChanged(int value);
    void dataBitsChanged(int value);
    void parityChanged(int value);
    void stopBitsChanged(int value);
    void flowControlChanged(int value);

private:
    void refreshDeviceConfig();
    void refreshDeviceConfigComboSignalsBlocked();

    SerialSocket& m_socket;
    Ui::SerialSocketConfigWidget m_ui;
    bool m_deviceEditForced = false;
    bool m_editMode = false;
};




}  // namespace serial_socket

}  // namespace plugin

}  // namespace cc_tools_qt



//
// Copyright 2023 - 2023 (C). Alex Robenko. All rights reserved.
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
#include "ui_SslSocketConfigWidget.h"
CC_ENABLE_WARNINGS()

#include "SslSocket.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace ssl_socket
{

namespace client
{

class SslSocketConfigWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Base;
public:
    typedef SslSocket::PortType PortType;

    explicit SslSocketConfigWidget(
        SslSocket& socket,
        QWidget* parentObj = nullptr);

    ~SslSocketConfigWidget() noexcept;

private slots:
    void hostValueChanged(const QString& value);
    void portValueChanged(int value);
    void caDirValueChanged(const QString& value);
    void caDirSelectClicked();
    void caDirFormatIndexChanged(int value);
    void caFileValueChanged(const QString& value);
    void caFileSelectClicked();
    void caFileFormatIndexChanged(int value);
    void verifyHostIndexChanged(int value);
    void verifyNameValueChanged(const QString& value);
    void sslProtocolIndexChanged(int value);
    void certValueChanged(const QString& value);
    void certSelectClicked();
    void certFormatIndexChanged(int value);
    void privKeyValueChanged(const QString& value);
    void privKeySelectClicked();
    void privKeyAlgIndexChanged(int value);
    void privKeyFormatIndexChanged(int value);
    void privKeyPassValueChanged(const QString& value);

private:
    SslSocket& m_socket;
    Ui::SslClientSocketConfigWidget m_ui;
};

}  // namespace client

}  // namespace ssl_socket

}  // namespace plugin

}  // namespace cc_tools_qt



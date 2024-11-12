//
// Copyright 2023 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "SslClientSocket.h"

#include "ui_SslClientSocketConfigWidget.h"

#include <QtWidgets/QWidget>


namespace cc_tools_qt
{

namespace plugin
{

class SslClientSocketConfigWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    using PortType = SslClientSocket::PortType;

    explicit SslClientSocketConfigWidget(
        SslClientSocket& socket,
        QWidget* parentObj = nullptr);

    ~SslClientSocketConfigWidget() noexcept;

private slots:
    void refresh();
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
    void privKeyShowHideClicked(bool checked);

private:
    SslClientSocket& m_socket;
    Ui::SslClientSocketConfigWidget m_ui;
};

}  // namespace plugin

}  // namespace cc_tools_qt



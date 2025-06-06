//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#include <memory>

#include <QtWidgets/QWidget>

#include "ui_MsgDetailsWidget.h"

#include "cc_tools_qt/ToolsMessage.h"
#include "DefaultMessageDisplayHandler.h"

namespace cc_tools_qt
{

class MsgDetailsWidget: public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    MsgDetailsWidget(QWidget* parentObj = nullptr);

public slots:
    void setEditEnabled(bool enabled);
    void displayMessage(ToolsMessagePtr msg);
    void updateTitle(ToolsMessagePtr msg);
    void clear();
    void refresh();

signals:
    void sigMsgUpdated();

private slots:
    void widgetScrolled(int value);
    void msgUpdated();

private:
    Ui::MsgDetailsWidget m_ui;
    DefaultMessageDisplayHandler m_msgDisplayHandler;
    MessageWidget* m_displayedMsgWidget = nullptr;
    ToolsMessagePtr m_displayedMsg;
    bool m_editEnabled = true;
};

} /* namespace cc_tools_qt */

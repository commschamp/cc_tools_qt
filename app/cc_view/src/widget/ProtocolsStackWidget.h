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

#include "ui_ProtocolsStackWidget.h"

#include "cc_tools_qt/ToolsMessage.h"

#include <QtWidgets/QWidget>

namespace cc_tools_qt
{

class ProtocolsStackWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    ProtocolsStackWidget(QWidget* parentObj = nullptr);
    ~ProtocolsStackWidget() noexcept;

    void displayMessage(ToolsMessagePtr msg, bool force);
    void clear();
    bool isAppMessageSelected();
    bool isExtraInfoSelected();

signals:
    void sigMessageSelected(ToolsMessagePtr msg, bool editEnabled);

private slots:
    void newItemSelected();

private:
    void reportMessageSelected(QTreeWidgetItem* item);
    static ToolsMessagePtr msgFromItem(QTreeWidgetItem* item);

    Ui::ProtocolsStackWidget m_ui;
};

}  // namespace cc_tools_qt


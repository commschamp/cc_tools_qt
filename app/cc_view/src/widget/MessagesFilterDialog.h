//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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

#include <QtWidgets/QDialog>

#include "ui_MessagesFilterDialog.h"

#include "cc_tools_qt/ToolsProtocol.h"
#include "GuiAppMgr.h"

namespace cc_tools_qt
{

class MessagesFilterDialog : public QDialog
{
    Q_OBJECT
    using Base = QDialog;
public:
    using FilteredMessages = GuiAppMgr::FilteredMessages;

    MessagesFilterDialog(
        FilteredMessages& filteredMessages,
        ToolsProtocolPtr protocol,
        QWidget* parentObj = nullptr);

private slots:
    void refreshDisplayedLists(const QString& searchText);
    void accept();
    void hideAllClicked();
    void hideSelectedClicked();
    void showSelectedClicked();
    void showAllClicked();
    void clearSearchText();
    void visibleSelected();
    void hiddenSelected();
    void hideVisible(QListWidgetItem* item);
    void showHidden(QListWidgetItem* item);
    void refreshButtons();

private:
    void refreshMessagesList(const MessagesFilterDialog::FilteredMessages& messages, QListWidget& list, QScrollArea& scroll);
    void refreshMessages();
    void refresh();
    void moveAllMessages(FilteredMessages& from, FilteredMessages& to);
    void moveSelectedMessages(QListWidget& list, FilteredMessages& from, FilteredMessages& to);
    void moveSelectedItem(const QListWidgetItem* item, FilteredMessages& from, FilteredMessages& to, bool finalUpdate = true);

    FilteredMessages& m_filteredMessages;
    FilteredMessages m_visibleMessages;
    FilteredMessages m_hiddenMessages;
    QString m_searchText;
    Ui::MessagesFilterDialog m_ui;
};

}  // namespace cc_tools_qt


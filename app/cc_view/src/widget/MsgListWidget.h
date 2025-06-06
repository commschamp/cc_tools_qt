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

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtCore/qnamespace.h>

#include "ui_MsgListWidget.h"

#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsProtocol.h"

#include "GuiAppMgr.h"

namespace cc_tools_qt
{

class ToolsMessage;

class MsgListWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    typedef GuiAppMgr::MsgType MsgType;

    MsgListWidget(
        const QString& title,
        QWidget* toolbar,
        QWidget* parentObj = nullptr);

protected slots:
    void addMessage(ToolsMessagePtr msg);
    void updateCurrentMessage(ToolsMessagePtr msg);
    void deleteCurrentMessage();
    void selectOnAdd(bool enabled);
    void clearSelection();
    void clearList(bool reportDeleted);
    void clearList();
    void stateChanged(int state);
    void moveSelectedTop();
    void moveSelectedUp();
    void moveSelectedDown();
    void moveSelectedBottom();
    void titleNeedsUpdate();
    void loadMessages(bool clearExisting, const QString& filename, ToolsProtocolPtr protocol);
    void saveMessages(const QString& filename);
    void selectMsg(int idx);

protected:
    virtual void msgClickedImpl(ToolsMessagePtr msg, int idx);
    virtual void msgDoubleClickedImpl(ToolsMessagePtr msg, int idx);
    virtual void msgListClearedImpl(ToolsMessagesList&& msgsList);
    virtual QString msgPrefixImpl(const ToolsMessage& msg) const;
    virtual const QString& msgTooltipImpl() const;
    virtual void stateChangedImpl(int state);
    virtual Qt::GlobalColor getItemColourImpl(MsgType type, bool valid) const;
    virtual void msgMovedImpl(int idx);
    virtual QString getTitleImpl() const;
    virtual void loadMessagesImpl(const QString& filename, ToolsProtocol& protocol);
    virtual void saveMessagesImpl(const QString& filename);

    ToolsMessagePtr currentMsg() const;
    ToolsMessagesList allMsgs() const;

private slots:
    void itemClicked(QListWidgetItem* item);
    void currentItemChanged(QListWidgetItem* current, QListWidgetItem* prev);
    void itemDoubleClicked(QListWidgetItem* item);
    void msgCommentUpdated(ToolsMessagePtr msg);

private:
    ToolsMessagePtr getMsgFromItem(QListWidgetItem* item) const;
    QString getMsgNameText(ToolsMessagePtr msg);
    Qt::GlobalColor defaultItemColour(bool valid) const;
    void moveItem(int fromRow, int toRow);
    void updateTitle();
    void processClick(QListWidgetItem* item);

    Ui::MsgListWidget m_ui;
    bool m_selectOnAdd = false;
    QString m_title;
    qint64 m_lastSelectionTimestamp = 0;
    QListWidgetItem* m_selectedItem = nullptr;
};

}  // namespace cc_tools_qt



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

#include "MsgListWidget.h"

#include <cassert>

#include <QtCore/QVariant>
#include <QtCore/QDateTime>

#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/property/message.h"
#include "GuiAppMgr.h"

namespace cc_tools_qt
{

namespace
{

const QString EmptyStr;

}  // namespace

MsgListWidget::MsgListWidget(
    const QString& title,
    QWidget* toolbar,
    QWidget* parentObj)
  : Base(parentObj),
    m_title(title)
{
    m_ui.setupUi(this);
    m_ui.m_groupBoxLayout->insertWidget(0, toolbar);
    updateTitle();

    m_ui.m_listWidget->setUniformItemSizes(true);
    connect(
        m_ui.m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(itemClicked(QListWidgetItem*)));
    connect(
        m_ui.m_listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
        this, SLOT(currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
    connect(
        m_ui.m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(itemDoubleClicked(QListWidgetItem*)));

    auto* guiMgr = GuiAppMgr::instance();
    assert(guiMgr != nullptr);
    connect(
        guiMgr, SIGNAL(sigMsgCommentUpdated(ToolsMessagePtr)),
        this, SLOT(msgCommentUpdated(ToolsMessagePtr)));
}

void MsgListWidget::addMessage(ToolsMessagePtr msg)
{
    assert(msg);
    m_ui.m_listWidget->addItem(getMsgNameText(msg));
    auto* item = m_ui.m_listWidget->item(m_ui.m_listWidget->count() - 1);
    item->setToolTip(msgTooltipImpl());

    bool valid = msg->isValid();

    auto type = property::message::ToolsMsgType().getFrom(*msg);
    if ((type != MsgType::Invalid) && (!msg->idAsString().isEmpty())) {
        item->setForeground(getItemColourImpl(type, valid));
    }
    else {
        item->setForeground(defaultItemColour(valid));
    }

    item->setData(
        Qt::UserRole,
        QVariant::fromValue(msg));

    if (m_selectOnAdd) {
        m_ui.m_listWidget->blockSignals(true);
        m_ui.m_listWidget->setCurrentRow(m_ui.m_listWidget->count() - 1);
        m_ui.m_listWidget->blockSignals(false);
        assert(m_ui.m_listWidget->currentItem() == item);
    }

    if (m_ui.m_listWidget->currentRow() < 0) {
        m_ui.m_listWidget->scrollToBottom();
    }

    updateTitle();
}

void MsgListWidget::updateCurrentMessage(ToolsMessagePtr msg)
{
    auto* item = m_ui.m_listWidget->currentItem();
    if (item == nullptr) {
        [[maybe_unused]] static constexpr bool No_item_is_selected_for_update = false;
        assert(No_item_is_selected_for_update);
        return;
    }

    item->setData(
        Qt::UserRole,
        QVariant::fromValue(msg));

    item->setText(getMsgNameText(msg));

    if ((!msg) || (msg->idAsString().isEmpty())) {
        item->setForeground(defaultItemColour(false));
        return;
    }

    bool valid = msg->isValid();

    auto type = property::message::ToolsMsgType().getFrom(*msg);
    if (type != MsgType::Invalid) {
        item->setForeground(getItemColourImpl(type, valid));
    }
    else {
        item->setForeground(defaultItemColour(valid));
    }
}

void MsgListWidget::deleteCurrentMessage()
{
    auto* item = m_ui.m_listWidget->currentItem();
    if (item == nullptr) {
        [[maybe_unused]] static constexpr bool No_item_is_selected_for_deletion = false;
        assert(No_item_is_selected_for_deletion);        
        return;
    }

    m_ui.m_listWidget->blockSignals(true);
    delete item; // will remove from the list
    m_ui.m_listWidget->blockSignals(false);


    updateTitle();

    auto* nextItem = m_ui.m_listWidget->currentItem();
    if (nextItem != nullptr) {
        processClick(nextItem);
    }
}

void MsgListWidget::selectOnAdd(bool enabled)
{
    m_selectOnAdd = enabled;
}

void MsgListWidget::clearSelection()
{
    m_ui.m_listWidget->clearSelection();
    m_ui.m_listWidget->setCurrentRow(-1);
}

void MsgListWidget::clearList(bool reportDeleted)
{
    ToolsMessagesList msgsList;
    if (reportDeleted) {
        auto count = m_ui.m_listWidget->count();
        for (auto idx = 0; idx < count; ++idx) {
            auto* item = m_ui.m_listWidget->item(idx);
            auto msg = getMsgFromItem(item);
            msgsList.push_back(std::move(msg));
        }
    }

    clearList();

    if (reportDeleted) {
        msgListClearedImpl(std::move(msgsList));
    }
}

void MsgListWidget::clearList()
{
    m_ui.m_listWidget->clear();
    updateTitle();
}

void MsgListWidget::stateChanged(int state)
{
    stateChangedImpl(state);
}

void MsgListWidget::moveSelectedTop()
{
    auto curRow = m_ui.m_listWidget->currentRow();
    if (curRow <= 0) {
        [[maybe_unused]] static constexpr bool No_item_is_selected_or_moving_up_top_item = false;
        assert(No_item_is_selected_or_moving_up_top_item);         
        return;
    }

    moveItem(curRow, 0);
}

void MsgListWidget::moveSelectedUp()
{
    auto curRow = m_ui.m_listWidget->currentRow();
    if (curRow <= 0) {
        [[maybe_unused]] static constexpr bool No_item_is_selected_or_moving_up_top_item = false;
        assert(No_item_is_selected_or_moving_up_top_item);
        return;
    }

    moveItem(curRow, curRow - 1);
}

void MsgListWidget::moveSelectedDown()
{
    auto curRow = m_ui.m_listWidget->currentRow();
    if ((m_ui.m_listWidget->count() - 1) <= curRow) {
        [[maybe_unused]] static constexpr bool No_item_is_selected_or_moving_down_bottom_item = false;
        assert(No_item_is_selected_or_moving_down_bottom_item);        
        return;
    }

    moveItem(curRow, curRow + 1);
}

void MsgListWidget::moveSelectedBottom()
{
    auto curRow = m_ui.m_listWidget->currentRow();
    if ((m_ui.m_listWidget->count() - 1) <= curRow) {
        [[maybe_unused]] static constexpr bool No_item_is_selected_or_moving_down_bottom_item = false;
        assert(No_item_is_selected_or_moving_down_bottom_item); 
        return;
    }

    moveItem(curRow, m_ui.m_listWidget->count() - 1);
}

void MsgListWidget::titleNeedsUpdate()
{
    m_title = getTitleImpl();
    updateTitle();
}

void MsgListWidget::loadMessages(
    bool clearExisting,
    const QString& filename,
    ToolsProtocolPtr protocol)
{
    if (clearExisting) {
        clearList();
    }

    loadMessagesImpl(filename, *protocol);
}

void MsgListWidget::saveMessages(const QString& filename)
{
    saveMessagesImpl(filename);
}

void MsgListWidget::selectMsg(int idx)
{
    assert(idx < m_ui.m_listWidget->count());
    m_ui.m_listWidget->blockSignals(true);
    m_ui.m_listWidget->setCurrentRow(idx);
    m_ui.m_listWidget->blockSignals(false);
}

void MsgListWidget::msgClickedImpl([[maybe_unused]] ToolsMessagePtr msg, [[maybe_unused]] int idx)
{
}

void MsgListWidget::msgDoubleClickedImpl([[maybe_unused]] ToolsMessagePtr msg, [[maybe_unused]] int idx)
{
}

void MsgListWidget::msgListClearedImpl([[maybe_unused]] ToolsMessagesList&& msgs)
{
}

QString MsgListWidget::msgPrefixImpl([[maybe_unused]] const ToolsMessage& msg) const
{
    return QString();
}

const QString& MsgListWidget::msgTooltipImpl() const
{
    return EmptyStr;
}

void MsgListWidget::stateChangedImpl([[maybe_unused]] int state)
{
}

Qt::GlobalColor MsgListWidget::getItemColourImpl([[maybe_unused]] MsgType type, bool valid) const
{
    return defaultItemColour(valid);
}

void MsgListWidget::msgMovedImpl([[maybe_unused]] int idx)
{
}

QString MsgListWidget::getTitleImpl() const
{
    [[maybe_unused]] static constexpr bool Should_not_be_called = false;
    assert(Should_not_be_called);     
    return QString();
}

void MsgListWidget::loadMessagesImpl([[maybe_unused]] const QString& filename, [[maybe_unused]] ToolsProtocol& protocol)
{
}

void MsgListWidget::saveMessagesImpl([[maybe_unused]] const QString& filename)
{
}

ToolsMessagePtr MsgListWidget::currentMsg() const
{
    auto* item = m_ui.m_listWidget->currentItem();
    assert(item != nullptr);
    return getMsgFromItem(item);
}

ToolsMessagesList MsgListWidget::allMsgs() const
{
    ToolsMessagesList allMsgsList;
    for (auto idx = 0; idx < m_ui.m_listWidget->count(); ++idx) {
        auto* item = m_ui.m_listWidget->item(idx);
        auto msgPtr = getMsgFromItem(item);
        assert(msgPtr);
        allMsgsList.push_back(std::move(msgPtr));
    }
    return allMsgsList;
}

void MsgListWidget::itemClicked(QListWidgetItem* item)
{
    assert(item != nullptr);
    if (m_selectedItem == item) {
        assert(0 < m_lastSelectionTimestamp);
        auto timestamp = QDateTime::currentMSecsSinceEpoch();
        static const decltype(timestamp) MinThreshold = 250;
        if (timestamp < (m_lastSelectionTimestamp + MinThreshold)) {
            return;
        }
    }

    processClick(item);
}

void MsgListWidget::currentItemChanged(QListWidgetItem* current, [[maybe_unused]] QListWidgetItem* prev)
{
    m_selectedItem = current;
    if (current != nullptr) {
        m_lastSelectionTimestamp = QDateTime::currentMSecsSinceEpoch();
        processClick(current);
        return;
    }

    m_lastSelectionTimestamp = 0;
    return;
}

void MsgListWidget::itemDoubleClicked(QListWidgetItem* item)
{
    msgDoubleClickedImpl(
        getMsgFromItem(item),
        m_ui.m_listWidget->row(item));
}

void MsgListWidget::msgCommentUpdated(ToolsMessagePtr msg)
{
    assert(msg);
    auto item = m_ui.m_listWidget->currentItem();
    if (item == nullptr) {
        return;
    }

    if (msg == getMsgFromItem(item)) {
        item->setText(getMsgNameText(msg));
    }
}

ToolsMessagePtr MsgListWidget::getMsgFromItem(QListWidgetItem* item) const
{
    auto var = item->data(Qt::UserRole);
    assert(var.canConvert<ToolsMessagePtr>());
    return var.value<ToolsMessagePtr>();
}

QString MsgListWidget::getMsgNameText(ToolsMessagePtr msg)
{
    assert(msg);
    auto itemStr = msgPrefixImpl(*msg);
    if (!itemStr.isEmpty()) {
        itemStr.append(": ");
    }
    itemStr.append(msg->name());

    auto comment = property::message::ToolsMsgComment().getFrom(*msg);
    if (!comment.isEmpty()) {
        itemStr.append(" (" + comment + ")");
    }
    return itemStr;
}

Qt::GlobalColor MsgListWidget::defaultItemColour(bool valid) const
{
    if (valid) {
        return Qt::black;
    }
    return Qt::red;
}

void MsgListWidget::moveItem(int fromRow, int toRow)
{
    assert(fromRow < m_ui.m_listWidget->count());
    auto* item = m_ui.m_listWidget->takeItem(fromRow);
    assert(toRow <= m_ui.m_listWidget->count());
    m_ui.m_listWidget->insertItem(toRow, item);
    m_ui.m_listWidget->setCurrentRow(toRow);
    msgMovedImpl(toRow);
}

void MsgListWidget::updateTitle()
{
    auto title =
        m_title +
        QString(" [%1]").arg(m_ui.m_listWidget->count(), 1, 10, QChar('0'));
    m_ui.m_groupBox->setTitle(title);
}

void MsgListWidget::processClick(QListWidgetItem* item)
{
    msgClickedImpl(
        getMsgFromItem(item),
        m_ui.m_listWidget->row(item));
}


}  // namespace cc_tools_qt



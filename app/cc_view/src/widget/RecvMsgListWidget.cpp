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

#include "RecvMsgListWidget.h"

#include "GuiAppMgr.h"
#include "MsgFileMgrG.h"
#include "RecvAreaToolBar.h"

#include "cc_tools_qt/property/message.h"

#include <QtWidgets/QVBoxLayout>

#include <cassert>
#include <chrono>

namespace cc_tools_qt
{

RecvMsgListWidget::RecvMsgListWidget(QWidget* parentObj)
  : Base(getTitlePrefix(), new RecvAreaToolBar(), parentObj)
{
    auto* guiMgr = GuiAppMgr::instance();
    assert(guiMgr != nullptr);

    selectOnAdd(guiMgr->recvMsgListSelectOnAddEnabled());

    connect(
        guiMgr, &GuiAppMgr::sigAddRecvMsg,
        this, &RecvMsgListWidget::addMessage);
    connect(
        guiMgr, &GuiAppMgr::sigRecvMsgListSelectOnAddEnabled,
        this, &RecvMsgListWidget::selectOnAdd);
    connect(
        guiMgr, &GuiAppMgr::sigRecvMsgListClearSelection,
        this, &RecvMsgListWidget::clearSelection);
    connect(
        guiMgr, &GuiAppMgr::sigRecvDeleteSelectedMsg,
        this, &RecvMsgListWidget::deleteCurrentMessage);
    connect(
        guiMgr, &GuiAppMgr::sigRecvClear,
        this, qOverload<bool>(&RecvMsgListWidget::clearList));
    connect(
        guiMgr, &GuiAppMgr::sigRecvListTitleNeedsUpdate,
        this, &RecvMsgListWidget::titleNeedsUpdate);
    connect(
        guiMgr, &GuiAppMgr::sigRecvSaveMsgs,
        this, &RecvMsgListWidget::saveMessages);
}

void RecvMsgListWidget::msgClickedImpl(ToolsMessagePtr msg, int idx)
{
    GuiAppMgr::instance()->recvMsgClicked(msg, idx);
}

void RecvMsgListWidget::msgListClearedImpl(ToolsMessagesList&& msgs)
{
    GuiAppMgr::instance()->deleteMessages(std::move(msgs));
}

QString RecvMsgListWidget::msgPrefixImpl(const ToolsMessage& msg) const
{
    auto timestamp = property::message::ToolsMsgTimestamp().getFrom(msg);
    if (timestamp == 0U) {
        return QString();
    }

    return QString("[%1]").arg(timestamp, 1, 10, QChar('0'));
}

const QString& RecvMsgListWidget::msgTooltipImpl() const
{
    static const QString& Tooltip("Click to display");
    return Tooltip;
}

Qt::GlobalColor RecvMsgListWidget::getItemColourImpl(MsgType type, bool valid) const
{
    if (type == MsgType::Received) {
        return Base::getItemColourImpl(type, valid);
    }

    if (valid) {
        return Qt::darkGreen;
    }

    return Qt::darkRed;
}

QString RecvMsgListWidget::getTitleImpl() const
{
    return getTitlePrefix();
}

void RecvMsgListWidget::saveMessagesImpl(const QString& filename)
{
    MsgFileMgrG::instanceRef().save(ToolsMsgFileMgr::Type::Recv, filename, allMsgs());
}

QString RecvMsgListWidget::getTitlePrefix()
{
    auto* guiAppMgr = GuiAppMgr::instance();
    assert(guiAppMgr != nullptr);

    if (guiAppMgr->recvListShowsReceived() && guiAppMgr->recvListShowsSent()) {
        static const QString Str("All Messages");
        return Str;
    }

    if (guiAppMgr->recvListShowsReceived()) {
        static const QString Str("Received Messages");
        return Str;
    }

    if (guiAppMgr->recvListShowsSent()) {
        static const QString Str("Sent Messages");
        return Str;
    }

    static const QString Str("No Messages");
    return Str;
}

} // namespace cc_tools_qt

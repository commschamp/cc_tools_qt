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

#include "SendMsgListWidget.h"

#include "GuiAppMgr.h"
#include "MsgFileMgrG.h"
#include "SendAreaToolBar.h"

#include "cc_tools_qt/property/message.h"

#include <QtWidgets/QVBoxLayout>

#include <cassert>

namespace cc_tools_qt
{

SendMsgListWidget::SendMsgListWidget(QWidget* parentObj)
  : Base("Messages to Send", new SendAreaToolBar(), parentObj),
    m_state(GuiAppMgr::instanceRef().sendState())
{
    selectOnAdd(true);

    auto* guiMgr = GuiAppMgr::instance();
    assert(guiMgr != nullptr);
    connect(
        guiMgr, &GuiAppMgr::sigAddSendMsg,
        this, &SendMsgListWidget::addMessage);
    connect(
        guiMgr, &GuiAppMgr::sigSendMsgUpdated,
        this, &SendMsgListWidget::updateCurrentMessage);
    connect(
        guiMgr, &GuiAppMgr::sigSendDeleteSelectedMsg,
        this, &SendMsgListWidget::deleteCurrentMessage);
    connect(
        guiMgr, &GuiAppMgr::sigSendMsgListClearSelection,
        this, &SendMsgListWidget::clearSelection);
    connect(
        guiMgr, &GuiAppMgr::sigSendClear,
        this, qOverload<>(&SendMsgListWidget::clearList));
    connect(
        guiMgr, &GuiAppMgr::sigSetSendState,
        this, &SendMsgListWidget::stateChanged);
    connect(
        guiMgr, &GuiAppMgr::sigSendMoveSelectedTop,
        this, &SendMsgListWidget::moveSelectedTop);
    connect(
        guiMgr, &GuiAppMgr::sigSendMoveSelectedUp,
        this, &SendMsgListWidget::moveSelectedUp);
    connect(
        guiMgr, &GuiAppMgr::sigSendMoveSelectedDown,
        this, &SendMsgListWidget::moveSelectedDown);
    connect(
        guiMgr, &GuiAppMgr::sigSendMoveSelectedBottom,
        this, &SendMsgListWidget::moveSelectedBottom);
    connect(
        guiMgr, &GuiAppMgr::sigSendLoadMsgs,
        this, &SendMsgListWidget::loadMessages);
    connect(
        guiMgr, &GuiAppMgr::sigSendSaveMsgs,
        this, &SendMsgListWidget::saveMessages);
    connect(
        guiMgr, &GuiAppMgr::sigSendMsgSelected,
        this, &SendMsgListWidget::selectMsg);
}

void SendMsgListWidget::msgClickedImpl(ToolsMessagePtr msg, int idx)
{
    GuiAppMgr::instance()->sendMsgClicked(std::move(msg), idx);
}

void SendMsgListWidget::msgDoubleClickedImpl(ToolsMessagePtr msg, int idx)
{
    if (m_state != State::Idle) {
        return;
    }
    GuiAppMgr::instance()->sendMsgDoubleClicked(std::move(msg), idx);
}

QString SendMsgListWidget::msgPrefixImpl(const ToolsMessage& msg) const
{
    QString str;
    do {
        auto delay = property::message::ToolsMsgDelay().getFrom(msg);
        auto repeatDur = property::message::ToolsMsgRepeatDuration().getFrom(msg);
        auto repeatCount = property::message::ToolsMsgRepeatCount().getFrom(msg, 1U);

        str =
            QString("(%1:%2:%3)").
                arg(delay, 1, 10, QChar('0')).
                arg(repeatDur, 1, 10, QChar('0')).
                arg(repeatCount, 1, 10, QChar('0'));

    } while (false);
    return str;
}

const QString& SendMsgListWidget::msgTooltipImpl() const
{
    static const QString& Tooltip("Click to display, double click to edit");
    return Tooltip;
}

void SendMsgListWidget::stateChangedImpl(int state)
{
    m_state = static_cast<State>(state);
    assert(m_state < State::NumOfStates);
    if (m_state == State::Idle) {
        return;
    }

    if (m_state == State::SendingSingle) {
        auto msg = currentMsg();
        assert(msg);
        ToolsMessagesList allMsgsList;
        allMsgsList.push_back(std::move(msg));
        GuiAppMgr::instanceRef().sendMessages(std::move(allMsgsList));
        return;
    }

    assert(m_state == State::SendingAll);
    auto allMsgsList = allMsgs();
    assert(!allMsgsList.empty());
    GuiAppMgr::instanceRef().sendMessages(std::move(allMsgsList));
}

void SendMsgListWidget::msgMovedImpl(int idx)
{
    GuiAppMgr::instanceRef().sendSelectedMsgMoved(idx);
}

void SendMsgListWidget::loadMessagesImpl(const QString& filename, ToolsProtocol& protocol)
{
    auto msgs = MsgFileMgrG::instanceRef().load(ToolsMsgFileMgr::Type::Send, filename, protocol);
    for (auto& m : msgs) {
        addMessage(m);
    }
    GuiAppMgr::instanceRef().sendUpdateList(allMsgs());
}

void SendMsgListWidget::saveMessagesImpl(const QString& filename)
{
    MsgFileMgrG::instanceRef().save(ToolsMsgFileMgr::Type::Send, filename, allMsgs());
}

} // namespace cc_tools_qt

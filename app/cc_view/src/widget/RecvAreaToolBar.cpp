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

#include "RecvAreaToolBar.h"

#include "ActionWrap.h"
#include "ShortcutMgr.h"
#include "icon.h"

#include <QtCore/QObject>
#include <QtGui/QIcon>

#include <cassert>

namespace cc_tools_qt
{

namespace
{

const QString StartTooltip("Start Reception");
const QString StopTooltip("Stop Reception");

QAction* createStartButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::start(), StartTooltip);
    return action;
}

QAction* createLoadButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::upload(), "Load Messages");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvLoadClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_LoadRecv);
    return action;
}


QAction* createSaveButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::save(), "Save Messages");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvSaveClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_SaveRecv);
    return action;
}

QAction* createCommentButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::comment(), "Add/Edit Message Comment");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvCommentClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_Comment);
    return action;
}

QAction* createDupButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::dup(), "Duplicate Message to Send Area");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvDupClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_DupMessage);
    return action;
}

QAction* createDeleteButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::remove(), "Delete Selected Message");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvDeleteClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_Delete);
    return action;
}

QAction* createClearButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::editClear(), "Delete All Displayed Messages");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvClearClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_ClearRecv);
    return action;
}

QAction* createEditFilterButton(QToolBar& bar)
{
    auto* action = bar.addAction(icon::filterEdit(), "Filter Displayed Messages");
    QObject::connect(
        action, &QAction::triggered,
        GuiAppMgr::instance(), &GuiAppMgr::recvEditFilterClicked);
    ShortcutMgr::instanceRef().updateShortcut(*action, ShortcutMgr::Key_EditFilter);
    return action;
}

QAction* createApplyFilter(QToolBar& bar)
{
    auto guiAppMgr = GuiAppMgr::instance();
    auto* action = bar.addAction(
        icon::filterApply(), "Apply Filter");
    action->setCheckable(true);
    action->setChecked(guiAppMgr->recvListApplyFilter());
    QObject::connect(
        action, &QAction::triggered,
        guiAppMgr, &GuiAppMgr::recvApplyFilterToggled);
    return action;
}

QAction* createShowGarbage(QToolBar& bar)
{
    auto guiAppMgr = GuiAppMgr::instance();
    auto* action = bar.addAction(
        icon::errorLog(), "Show Message Errors and Garbage Data");
    action->setCheckable(true);
    action->setChecked(guiAppMgr->recvListShowsGarbage());
    QObject::connect(
        action, &QAction::triggered,
        guiAppMgr, &GuiAppMgr::recvShowGarbageToggled);
    return action;
}

QAction* createShowReceived(QToolBar& bar)
{
    auto guiAppMgr = GuiAppMgr::instance();
    auto* action = bar.addAction(icon::showRecv(), "Show Received Messages");
    action->setCheckable(true);
    action->setChecked(guiAppMgr->recvListShowsReceived());
    QObject::connect(
        action, &QAction::triggered,
        guiAppMgr, &GuiAppMgr::recvShowRecvToggled);
    return action;
}

QAction* createShowSent(QToolBar& bar)
{
    auto guiAppMgr = GuiAppMgr::instance();
    auto* action = bar.addAction(icon::showSent(), "Show Sent Messages");
    action->setCheckable(true);
    action->setChecked(guiAppMgr->recvListShowsSent());
    QObject::connect(
        action, &QAction::triggered,
        guiAppMgr, &GuiAppMgr::recvShowSentToggled);
    return action;
}

}  // namespace

RecvAreaToolBar::RecvAreaToolBar(QWidget* parentObj)
  : Base(parentObj),
    m_startStopButton(createStartButton(*this)),
    m_loadButton(createLoadButton(*this)),
    m_saveButton(createSaveButton(*this)),
    m_commentButton(createCommentButton(*this)),
    m_dupButton(createDupButton(*this)),
    m_deleteButton(createDeleteButton(*this)),
    m_clearButton(createClearButton(*this)),
    m_editFilterButton(createEditFilterButton(*this)),
    m_applyFilterButton(createApplyFilter(*this)),
    m_showGarbageButton(createShowGarbage(*this)),
    m_showRecvButton(createShowReceived(*this)),
    m_showSentButton(createShowSent(*this)),
    m_state(GuiAppMgr::instance()->recvState()),
    m_sendState(GuiAppMgr::instance()->sendState()),
    m_activeState(GuiAppMgr::instance()->getActivityState())
{
    insertSeparator(m_applyFilterButton);
    auto empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    insertWidget(m_applyFilterButton, empty);

    connect(
        m_startStopButton, &QAction::triggered,
        this, &RecvAreaToolBar::startStopClicked);

    auto* guiAppMgr = GuiAppMgr::instance();
    connect(
        guiAppMgr, &GuiAppMgr::sigRecvListCountReport,
        this, &RecvAreaToolBar::recvListCountReport);

    connect(
        guiAppMgr, &GuiAppMgr::sigRecvMsgSelected,
        this, &RecvAreaToolBar::recvMsgSelectedReport);

    connect(
        guiAppMgr, &GuiAppMgr::sigSetRecvState,
        this, &RecvAreaToolBar::recvStateChanged);

    connect(
        guiAppMgr, &GuiAppMgr::sigSetSendState,
        this, &RecvAreaToolBar::sendStateChanged);

    connect(
        guiAppMgr, &GuiAppMgr::sigActivityStateChanged,
        this, &RecvAreaToolBar::activeStateChanged);

    refresh();
}

void RecvAreaToolBar::startStopClicked()
{
    if (m_state == State::Idle) {
        GuiAppMgr::instance()->recvStartClicked();
        return;
    }

    assert(m_state == State::Running);
    GuiAppMgr::instance()->recvStopClicked();
}

void RecvAreaToolBar::recvListCountReport(unsigned count)
{
    m_listTotal = count;
    refresh();
}

void RecvAreaToolBar::recvMsgSelectedReport(int idx)
{
    m_selectedIdx = idx;
    refresh();
}

void RecvAreaToolBar::recvStateChanged(int state)
{
    auto castedState = static_cast<State>(state);
    if (m_state == castedState) {
        return;
    }

    m_state = castedState;
    refresh();
    return;
}

void RecvAreaToolBar::sendStateChanged(int state)
{
    auto castedState = static_cast<SendState>(state);
    if (m_sendState == castedState) {
        return;
    }

    m_sendState = castedState;
    refresh();
    return;
}

void RecvAreaToolBar::activeStateChanged(int state)
{
    auto castedState = static_cast<ActivityState>(state);
    if (m_activeState == castedState) {
        return;
    }

    m_activeState = castedState;
    refresh();
}

void RecvAreaToolBar::refresh()
{
    refreshStartStopButton();
    refreshLoadButton();
    refreshSaveButton();
    refreshCommentButton();
    refreshDupButton();
    refreshDeleteButton();
    refreshClearButton();
    refreshEditFilterButton();
}

void RecvAreaToolBar::refreshStartStopButton()
{
    auto* button = m_startStopButton;
    assert(button != nullptr);
    bool enabled = (m_activeState == ActivityState::Active);
    if (m_state == State::Running) {
        button->setIcon(icon::stop());
        button->setText(StopTooltip);
    }
    else {
        button->setIcon(icon::start());
        button->setText(StartTooltip);
    }
    button->setEnabled(enabled);
    ShortcutMgr::instanceRef().updateShortcut(*button, ShortcutMgr::Key_Receive);
}

void RecvAreaToolBar::refreshLoadButton()
{
    auto* button = m_loadButton;
    assert(button != nullptr);
    bool enabled = false;
    do {
        if ((m_activeState != ActivityState::Active) ||
            (m_state != State::Idle)) {
            break;
        }

        if ((m_showSentButton->isChecked()) &&
            (m_sendState != SendState::Idle)) {
            break;
        }

        enabled = true;
    } while (false);
    button->setEnabled(enabled);
}

void RecvAreaToolBar::refreshSaveButton()
{
    auto* button = m_saveButton;
    assert(button != nullptr);
    bool enabled = false;
    do {
        if ((m_activeState != ActivityState::Active) ||
            (m_state != State::Idle) ||
            (listEmpty())) {
            break;
        }

        if ((m_showSentButton->isChecked()) &&
            (m_sendState != SendState::Idle)) {
            break;
        }

        enabled = true;
    } while (false);
    button->setEnabled(enabled);
}

void RecvAreaToolBar::refreshCommentButton()
{
    auto* button = m_commentButton;
    assert(button);
    bool enabled =
        (m_activeState == ActivityState::Active) &&
        msgSelected();
    button->setEnabled(enabled);
}

void RecvAreaToolBar::refreshDupButton()
{
    auto* button = m_dupButton;
    assert(button);
    bool enabled =
        (m_activeState == ActivityState::Active) &&
        (msgSelected());
    button->setEnabled(enabled);
}

void RecvAreaToolBar::refreshDeleteButton()
{
    auto* button = m_deleteButton;
    assert(button);
    bool enabled =
        (m_activeState == ActivityState::Active) &&
        msgSelected();
    button->setEnabled(enabled);
}

void RecvAreaToolBar::refreshClearButton()
{
    auto* button = m_clearButton;
    assert(button);
    bool enabled =
        (m_activeState == ActivityState::Active) &&
        (!listEmpty());
    button->setEnabled(enabled);
}

void RecvAreaToolBar::refreshEditFilterButton()
{
    auto* button = m_editFilterButton;
    assert(button);
    bool enabled =
        (m_activeState == ActivityState::Active);
    button->setEnabled(enabled);
}

bool RecvAreaToolBar::msgSelected() const
{
    bool result = (0 <= m_selectedIdx);
    assert(m_selectedIdx < static_cast<decltype(m_selectedIdx)>(m_listTotal));
    return result;
}

bool RecvAreaToolBar::listEmpty() const
{
    return 0 == m_listTotal;
}


}  // namespace cc_tools_qt


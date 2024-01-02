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

#include "RightPaneWidget.h"

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtWidgets/QVBoxLayout>
CC_ENABLE_WARNINGS()

#include "DefaultMessageDisplayWidget.h"
#include "GuiAppMgr.h"
#include "MsgMgrG.h"

namespace cc_tools_qt
{

RightPaneWidget::RightPaneWidget(QWidget* parentObj)
  : Base(parentObj)
{
    m_displayWidget = new DefaultMessageDisplayWidget();
    m_displayWidget->setEditEnabled(false);

    auto* guiAppMgr = GuiAppMgr::instance();
    connect(guiAppMgr, SIGNAL(sigDisplayMsg(MessagePtr)),
            m_displayWidget, SLOT(displayMessage(MessagePtr)));
    connect(guiAppMgr, SIGNAL(sigDisplayMsg(MessagePtr)),
            this, SLOT(displayMessage(MessagePtr)));            
    connect(guiAppMgr, SIGNAL(sigClearDisplayedMsg()),
            m_displayWidget, SLOT(clear()));
    connect(m_displayWidget, SIGNAL(sigMsgUpdated()),
            this, SLOT(msgUpdated()));           
    auto* paneLayout = new QVBoxLayout();
    paneLayout->addWidget(m_displayWidget);
    setLayout(paneLayout);
}

void RightPaneWidget::displayMessage(MessagePtr msg)
{
    // Enable edit of the messages that haven't been sent or received yet, 
    // i.e. reside in the send area.
    m_displayedMsg = msg;
    auto type = property::message::Type().getFrom(*msg);
    m_displayWidget->setEditEnabled(type == Message::Type::Invalid);
}

void RightPaneWidget::displayMessagePostponed(MessagePtr msg, bool force)
{
    m_displayWidget->displayMessage(msg, force);
}

void RightPaneWidget::msgUpdated()
{
    auto& msgMgr = MsgMgrG::instanceRef();
    auto protocol = msgMgr.getProtocol();
    auto status = protocol->updateMessage(*m_displayedMsg);
    bool forceUpdate = (status == Protocol::UpdateStatus::Changed);

    // Direct invocation of displayMessage(std::move(msg))
    // in place here causes SIGSEGV. No idea why.
    QMetaObject::invokeMethod(
        this,
        "displayMessagePostponed",
        Qt::QueuedConnection,
        Q_ARG(cc_tools_qt::MessagePtr, m_displayedMsg),
        Q_ARG(bool, forceUpdate));    
}

}  // namespace cc_tools_qt



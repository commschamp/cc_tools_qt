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

#include "RightPaneWidget.h"

#include "DefaultMessageDisplayWidget.h"
#include "GuiAppMgr.h"
#include "MsgMgrG.h"

#include "cc_tools_qt/property/message.h"

#include <QtWidgets/QVBoxLayout>

namespace cc_tools_qt
{

RightPaneWidget::RightPaneWidget(QWidget* parentObj)
  : Base(parentObj)
{
    m_displayWidget = new DefaultMessageDisplayWidget();
    m_displayWidget->setEditEnabled(false);

    auto* guiAppMgr = GuiAppMgr::instance();
    connect(
        guiAppMgr, &GuiAppMgr::sigDisplayMsg,
        m_displayWidget,
        [this](ToolsMessagePtr msg)
        {
            m_displayWidget->displayMessage(msg);
        });

    connect(
        guiAppMgr, &GuiAppMgr::sigDisplayMsg,
        this, &RightPaneWidget::displayMessage);

    connect(
        guiAppMgr, &GuiAppMgr::sigClearDisplayedMsg,
        m_displayWidget, &MessageDisplayWidget::clear);

    connect(
        m_displayWidget, &MessageDisplayWidget::sigMsgUpdated,
        this, &RightPaneWidget::msgUpdated);

    auto* paneLayout = new QVBoxLayout();
    paneLayout->addWidget(m_displayWidget);
    setLayout(paneLayout);
}

void RightPaneWidget::displayMessage(ToolsMessagePtr msg)
{
    // Enable edit of the messages that haven't been sent or received yet,
    // i.e. reside in the send area.
    m_displayedMsg = msg;
    auto type = static_cast<ToolsMessage::Type>(cc_tools_qt::property::message::ToolsMsgType().getFrom(*msg));
    m_displayWidget->setEditEnabled(type == ToolsMessage::Type::Invalid);
}

void RightPaneWidget::displayMessagePostponed(ToolsMessagePtr msg, bool force)
{
    assert(m_displayWidget != nullptr);
    m_displayWidget->displayMessage(msg, force);
}

void RightPaneWidget::msgUpdated()
{
    auto& msgMgr = MsgMgrG::instanceRef();
    auto protocol = msgMgr.getProtocol();
    auto status = protocol->updateMessage(*m_displayedMsg);
    bool forceUpdate = (status == ToolsProtocol::UpdateStatus::Changed);
    GuiAppMgr::instance()->sendUpdateMessage(m_displayedMsg);

    // Direct invocation of displayMessage(std::move(msg))
    // in place here causes SIGSEGV. No idea why.
    QMetaObject::invokeMethod(
        this,
        [this, msgParam = m_displayedMsg, forceUpdate]()
        {
            displayMessagePostponed(std::move(msgParam), forceUpdate);
        },
        Qt::QueuedConnection
    );
}

}  // namespace cc_tools_qt


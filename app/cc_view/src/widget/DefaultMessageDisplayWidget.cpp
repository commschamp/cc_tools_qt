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

#include "DefaultMessageDisplayWidget.h"

#include "MsgDetailsWidget.h"
#include "ProtocolsStackWidget.h"

#include "cc_tools_qt/property/message.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>

#include <cassert>

namespace cc_tools_qt
{

DefaultMessageDisplayWidget::DefaultMessageDisplayWidget(QWidget* parentObj)
  : Base(parentObj),
    m_msgDetailsWidget(new MsgDetailsWidget()),
    m_protocolsDetailsWidget(new ProtocolsStackWidget())
{
    connect(
        m_protocolsDetailsWidget, &ProtocolsStackWidget::sigMessageSelected,
        this, &DefaultMessageDisplayWidget::msgSelectedInProtocol);

    connect(
        m_msgDetailsWidget, &MsgDetailsWidget::sigMsgUpdated,
        this, &DefaultMessageDisplayWidget::msgUpdated);

    auto* splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(m_msgDetailsWidget);
    splitter->addWidget(m_protocolsDetailsWidget);

    auto* widgetLayout = new QVBoxLayout();
    widgetLayout->addWidget(splitter);

    setLayout(widgetLayout);
}

void DefaultMessageDisplayWidget::displayMessageImpl(
    ToolsMessagePtr msg,
    bool force)
{
    assert(msg);
    m_displayedMsg = msg;
    m_msgDetailsWidget->updateTitle(msg);
    m_protocolsDetailsWidget->displayMessage(std::move(msg), force);
}

void DefaultMessageDisplayWidget::setEditEnabledImpl(bool enabled)
{
    m_globalEditEnabled = enabled;
    m_msgDetailsWidget->setEditEnabled(enabled);
}

void DefaultMessageDisplayWidget::clearImpl()
{
    m_msgDetailsWidget->clear();
    m_protocolsDetailsWidget->clear();
}

void DefaultMessageDisplayWidget::refreshImpl()
{
    if (m_msgDetailsWidget != nullptr) {
        m_msgDetailsWidget->refresh();
    }
}

void DefaultMessageDisplayWidget::msgSelectedInProtocol(
    ToolsMessagePtr msg,
    bool editEnabled)
{
    assert(msg);
    m_msgDetailsWidget->displayMessage(msg);
    m_msgDetailsWidget->setEditEnabled(m_globalEditEnabled && editEnabled);
}

void DefaultMessageDisplayWidget::msgUpdated()
{
    if (!m_protocolsDetailsWidget->isExtraInfoSelected()) {
        emit sigMsgUpdated();
        return;
    }

    assert(m_displayedMsg);
    if (!m_displayedMsg) {
        return;
    }

    auto extraInfoMsg = property::message::ToolsMsgExtraInfoMsg().getFrom(*m_displayedMsg);
    if ((!extraInfoMsg) || (!extraInfoMsg->isValid())) {
        return;
    }

    auto extraData = extraInfoMsg->encodeData();
    if (extraData.empty()) {
        property::message::ToolsMsgExtraInfo().setTo(QVariantMap(), *m_displayedMsg);
        return;
    }

    auto doc =
        QJsonDocument::fromJson(
            QByteArray(
                reinterpret_cast<const char*>(&extraData[0]),
                static_cast<int>(extraData.size())));
    property::message::ToolsMsgExtraInfo().setTo(doc.object().toVariantMap(), *m_displayedMsg);
}

}  // namespace cc_tools_qt


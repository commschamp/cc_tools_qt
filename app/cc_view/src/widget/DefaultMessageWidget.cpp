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

#include "DefaultMessageWidget.h"

#include <memory>
#include <cassert>

#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QFrame>
#include <QtCore/QVariant>

namespace cc_tools_qt
{

DefaultMessageWidget::DefaultMessageWidget(
    ToolsMessage& msg,
    QWidget* parentObj)
  : Base(parentObj),
    m_msg(msg),
    m_layout(new LayoutType())
{
    setLayout(m_layout);
    m_layout->addItem(
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    hide();
}

void DefaultMessageWidget::addExtraTransportFieldWidget(FieldWidget* field)
{
    if (field == nullptr) {
        [[maybe_unused]] static constexpr bool Field_object_should_be_provided = false;
        assert(Field_object_should_be_provided);
        return;
    }

    if (m_curExtraTransportFieldIdx != 0) {
        m_layout->insertWidget(m_layout->count() - 1, createFieldSeparator().release());
    }
    m_layout->insertWidget(m_layout->count() - 1, field);
    connectFieldSignals(field);

    ++m_curExtraTransportFieldIdx;
}

void DefaultMessageWidget::addFieldWidget(FieldWidget* field)
{
    if (field == nullptr) {
        [[maybe_unused]] static constexpr bool Field_object_should_be_provided = false;
        assert(Field_object_should_be_provided);
        return;
    }

    if ((m_curFieldIdx != 0) || (m_curExtraTransportFieldIdx != 0)) {
        m_layout->insertWidget(m_layout->count() - 1, createFieldSeparator().release());
    }
    m_layout->insertWidget(m_layout->count() - 1, field);
    connectFieldSignals(field);

    ++m_curFieldIdx;
}

void DefaultMessageWidget::refreshImpl()
{
    emit sigRefreshFields();
}

void DefaultMessageWidget::setEditEnabledImpl(bool enabled)
{
    emit sigSetEditEnabled(enabled);
}

std::unique_ptr<QFrame> DefaultMessageWidget::createFieldSeparator()
{
    std::unique_ptr<QFrame> line(new QFrame(this));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

void DefaultMessageWidget::connectFieldSignals(FieldWidget* fieldWidget)
{
    assert(fieldWidget != nullptr);
    connect(this, SIGNAL(sigRefreshFields()), fieldWidget, SLOT(refresh()));
    connect(this, SIGNAL(sigSetEditEnabled(bool)), fieldWidget, SLOT(setEditEnabled(bool)));
    connect(fieldWidget, SIGNAL(sigFieldUpdated()), this, SIGNAL(sigMsgUpdated()));
}

}  // namespace cc_tools_qt



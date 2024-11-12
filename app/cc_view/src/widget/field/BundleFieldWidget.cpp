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

#include "BundleFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <type_traits>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>

namespace cc_tools_qt
{

BundleFieldWidget::BundleFieldWidget(
    FieldPtr fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);

    commonConstruct();
}

BundleFieldWidget::~BundleFieldWidget() noexcept = default;

void BundleFieldWidget::addMemberField(FieldWidget* memberFieldWidget)
{
    m_members.push_back(memberFieldWidget);

    if (0 < m_ui.m_membersLayout->count()) {
        auto* line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        m_ui.m_membersLayout->addWidget(line);
    }

    m_ui.m_membersLayout->addWidget(memberFieldWidget);
    assert(static_cast<std::size_t>(m_ui.m_membersLayout->count()) == ((m_members.size() * 2) - 1));

    connect(
        memberFieldWidget, SIGNAL(sigFieldUpdated()),
        this, SLOT(memberFieldUpdated()));
}

ToolsField& BundleFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void BundleFieldWidget::refreshImpl()
{
    for (auto* memberFieldWidget : m_members) {
        memberFieldWidget->refresh();
    }
}

void BundleFieldWidget::editEnabledUpdatedImpl()
{
    bool enabled = isEditEnabled();
    for (auto* memberFieldWidget : m_members) {
        memberFieldWidget->setEditEnabled(enabled);
    }
}

void BundleFieldWidget::memberFieldUpdated()
{
    auto senderIter = std::find(m_members.begin(), m_members.end(), qobject_cast<FieldWidget*>(sender()));
    assert(senderIter != m_members.end());
    auto idx = static_cast<unsigned>(std::distance(m_members.begin(), senderIter));
    auto& memFields = m_fieldPtr->getMembers();
    assert(idx < memFields.size());
    auto& memWrapPtr = memFields[idx];
    if (!memWrapPtr->canWrite()) {
        memWrapPtr->reset();
        assert(memWrapPtr->canWrite());
        (*senderIter)->refresh();
    }

    emitFieldUpdated();
}

}  // namespace cc_tools_qt



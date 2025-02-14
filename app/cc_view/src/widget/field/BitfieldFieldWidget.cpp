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

#include "BitfieldFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <type_traits>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>

namespace cc_tools_qt
{

BitfieldFieldWidget::BitfieldFieldWidget(
    FieldPtr&& fieldPtr,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);

    assert(m_ui.m_serValueLineEdit != nullptr);
    setSerialisedInputMask(*m_ui.m_serValueLineEdit, m_fieldPtr->width());

    commonConstruct();

    connect(m_ui.m_serValueLineEdit, SIGNAL(textEdited(const QString&)),
            this, SLOT(serialisedValueUpdated(const QString&)));
}

BitfieldFieldWidget::~BitfieldFieldWidget() noexcept = default;

void BitfieldFieldWidget::addMemberField(FieldWidget* memberFieldWidget)
{
    m_members.push_back(memberFieldWidget);

    if (m_ui.m_membersLayout->count() != 0) {
        auto* line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        m_ui.m_membersLayout->addWidget(line);
    }

    m_ui.m_membersLayout->addWidget(memberFieldWidget);
    assert(static_cast<std::size_t>(m_ui.m_membersLayout->count()) == ((m_members.size() * 2) - 1));

    refreshInternal();

    connect(
        memberFieldWidget, SIGNAL(sigFieldUpdated()),
        this, SLOT(memberFieldUpdated()));
}

ToolsField& BitfieldFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void BitfieldFieldWidget::refreshImpl()
{
    refreshInternal();
    refreshMembers();
}

void BitfieldFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_serValueLineEdit->setReadOnly(readonly);
    for (auto* memberFieldWidget : m_members) {
        memberFieldWidget->setEditEnabled(!readonly);
    }
}


void BitfieldFieldWidget::serialisedValueUpdated(const QString& value)
{
    handleNumericSerialisedValueUpdate(value, *m_fieldPtr);
    refreshMembers();
}

void BitfieldFieldWidget::memberFieldUpdated()
{
    auto senderIter = std::find(m_members.begin(), m_members.end(), qobject_cast<FieldWidget*>(sender()));
    assert(senderIter != m_members.end());
    auto idx = static_cast<unsigned>(std::distance(m_members.begin(), senderIter));
    auto& members = m_fieldPtr->getMembers();
    assert(idx < members.size());
    auto& memFieldPtr = members[idx];
    if (!memFieldPtr->canWrite()) {
        memFieldPtr->reset();
        assert(memFieldPtr->canWrite());
        (*senderIter)->refresh();
    }

    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->reset();
        assert(m_fieldPtr->canWrite());
    }

    refreshInternal();
    emitFieldUpdated();
}

void BitfieldFieldWidget::refreshInternal()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValueLineEdit != nullptr);
    updateValue(*m_ui.m_serValueLineEdit, m_fieldPtr->getSerialisedString());

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValueLineEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void BitfieldFieldWidget::refreshMembers()
{
    for (auto* memberFieldWidget : m_members) {
        memberFieldWidget->refresh();
    }
}

}  // namespace cc_tools_qt



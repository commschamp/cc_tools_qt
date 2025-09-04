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

#include "ArrayListFieldWidget.h"

#include <algorithm>
#include <cassert>

namespace cc_tools_qt
{

ArrayListElementWidget::ArrayListElementWidget(
    FieldWidget* fieldWidget,
    QWidget* parentObj)
  : Base(parentObj)
{
    m_ui.setupUi(this);
    m_fieldWidget = fieldWidget;
    m_ui.m_layout->addWidget(fieldWidget);

    updateUi();

    connect(
        m_fieldWidget, &FieldWidget::sigFieldUpdated,
        this, &ArrayListElementWidget::sigFieldUpdated);

    connect(
        m_ui.m_removePushButton, &QPushButton::clicked,
        this, &ArrayListElementWidget::sigRemoveRequested);

}

void ArrayListElementWidget::refresh()
{
    m_fieldWidget->refresh();
}

void ArrayListElementWidget::setEditEnabled(bool enabled)
{
    m_editEnabled = enabled;
    m_fieldWidget->setEditEnabled(enabled);
    updateUi();
}

void ArrayListElementWidget::setDeletable(bool deletable)
{
    m_deletable = deletable;
    updateUi();
}

void ArrayListElementWidget::setNameSuffix(const QString& value)
{
    assert(m_fieldWidget != nullptr);
    m_fieldWidget->setNameSuffix(value);
}

void ArrayListElementWidget::updateUi()
{
    bool deleteButtonVisible = m_editEnabled && m_deletable;
    m_ui.m_buttonWidget->setVisible(deleteButtonVisible);
    m_ui.m_sepLine->setVisible(deleteButtonVisible);
}

ArrayListFieldWidget::ArrayListFieldWidget(
    FieldPtr fieldPtr,
    CreateMissingDataFieldsFunc&& updateFunc,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr)),
    m_createMissingDataFieldsCallback(std::move(updateFunc))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);

    commonConstruct();

    assert(m_fieldPtr->canWrite());
    refreshInternal();
    addMissingFields();

    updateUi();

    connect(
        m_ui.m_addFieldPushButton, &QPushButton::clicked,
        this, &ArrayListFieldWidget::addNewField);
}

ArrayListFieldWidget::~ArrayListFieldWidget() noexcept = default;

ToolsField& ArrayListFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void ArrayListFieldWidget::refreshImpl()
{
    while (!m_elements.empty()) {
        assert(m_elements.back() != nullptr);
        delete m_elements.back();
        m_elements.pop_back();
    }

    if (m_fieldPtr->hasFixedSize()) {
        m_fieldPtr->adjustFixedSize();
    }

    m_fieldPtr->refreshMembers();

    refreshInternal();
    addMissingFields();
    assert(m_elements.size() == m_fieldPtr->size());
}

void ArrayListFieldWidget::editEnabledUpdatedImpl()
{
    for (auto* elem : m_elements) {
        elem->setEditEnabled(isEditEnabled());
    }
    updateUi();
}

void ArrayListFieldWidget::dataFieldUpdated()
{
    if (!m_fieldPtr->canWrite()) {
        auto senderIter = std::find(m_elements.begin(), m_elements.end(), qobject_cast<ArrayListElementWidget*>(sender()));
        assert(senderIter != m_elements.end());
        auto idx = static_cast<unsigned>(std::distance(m_elements.begin(), senderIter));
        auto& memFields = m_fieldPtr->getMembers();
        assert(idx < memFields.size());
        auto& memWrapPtr = memFields[idx];
        if (!memWrapPtr->canWrite()) {
            memWrapPtr->reset();
            assert(memWrapPtr->canWrite());
            (*senderIter)->refresh();
        }

        if (!m_fieldPtr->canWrite()) {
            memWrapPtr->reset();
            assert(memWrapPtr->canWrite());
            (*senderIter)->refresh();
        }
    }

    refreshInternal();
    emitFieldUpdated();
}

void ArrayListFieldWidget::addNewField()
{
    m_fieldPtr->addField();
    refreshImpl();
    assert(m_elements.size() == m_fieldPtr->size());
    emitFieldUpdated();
}

void ArrayListFieldWidget::removeField()
{
    auto* sigSender = sender();
    auto iter = std::find(m_elements.begin(), m_elements.end(), sigSender);
    if (iter == m_elements.end()) {
        [[maybe_unused]] static constexpr bool Something_is_not_right = false;
        assert(Something_is_not_right);
        return;
    }

    auto idx = static_cast<int>(std::distance(m_elements.begin(), iter));

    m_fieldPtr->removeField(idx);

    refreshImpl();

    assert(m_elements.size() == m_fieldPtr->size());
    assert(m_elements.size() == static_cast<unsigned>(m_ui.m_membersLayout->count()));

    emitFieldUpdated();
}

void ArrayListFieldWidget::addDataField(FieldWidget* dataFieldWidget)
{
    dataFieldWidget->setNameSuffix(QString(" %1").arg(m_elements.size()));
    auto* fieldPtrWidget = new ArrayListElementWidget(dataFieldWidget);
    fieldPtrWidget->setEditEnabled(isEditEnabled());
    fieldPtrWidget->setDeletable(!m_fieldPtr->hasFixedSize());

    connect(
        fieldPtrWidget, &ArrayListElementWidget::sigFieldUpdated,
        this, &ArrayListFieldWidget::dataFieldUpdated);

    connect(
        fieldPtrWidget, &ArrayListElementWidget::sigRemoveRequested,
        this, &ArrayListFieldWidget::removeField);

    m_elements.push_back(fieldPtrWidget);
    m_ui.m_membersLayout->addWidget(fieldPtrWidget);
}

void ArrayListFieldWidget::refreshInternal()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValuePlainTextEdit != nullptr);
    updateSerValue(*m_ui.m_serValuePlainTextEdit, *m_fieldPtr);

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValuePlainTextEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void ArrayListFieldWidget::updateUi()
{
    bool addButtonVisible = isEditEnabled() && (!m_fieldPtr->hasFixedSize());
    m_ui.m_addSepLine->setVisible(addButtonVisible);
    m_ui.m_addFieldPushButton->setVisible(addButtonVisible);
}

void ArrayListFieldWidget::addMissingFields()
{
    if (!m_createMissingDataFieldsCallback) {
        [[maybe_unused]] static constexpr bool Callback_should_exist = false;
        assert(Callback_should_exist);
        return;
    }

    assert(m_elements.empty());
    auto fieldWidgets = m_createMissingDataFieldsCallback(*m_fieldPtr);
    for (auto& fieldWidgetPtr : fieldWidgets) {
        addDataField(fieldWidgetPtr.release());
    }

    assert(m_elements.size() == m_fieldPtr->size());
    assert(m_elements.size() == static_cast<unsigned>(m_ui.m_membersLayout->count()));
}

}  // namespace cc_tools_qt



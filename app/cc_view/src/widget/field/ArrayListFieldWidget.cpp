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

#include "ArrayListFieldWidget.h"

#include <algorithm>
#include <cassert>

#include "cc_tools_qt/property/field.h"

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
        m_fieldWidget, SIGNAL(sigFieldUpdated()),
        this, SIGNAL(sigFieldUpdated()));

    connect(
        m_ui.m_removePushButton, SIGNAL(clicked()),
        this, SIGNAL(sigRemoveRequested()));

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

void ArrayListElementWidget::updateProperties(const QVariantMap& props)
{
    assert(m_fieldWidget != nullptr);
    m_fieldWidget->updateProperties(props);
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
    WrapperPtr wrapper,
    CreateMissingDataFieldsFunc&& updateFunc,
    QWidget* parentObj)
  : Base(parentObj),
    m_wrapper(std::move(wrapper)),
    m_createMissingDataFieldsCallback(std::move(updateFunc))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);

    assert(m_wrapper->canWrite());
    refreshInternal();
    addMissingFields();

    updateUi();

    connect(
        m_ui.m_addFieldPushButton, SIGNAL(clicked()),
        this, SLOT(addNewField()));
}

ArrayListFieldWidget::~ArrayListFieldWidget() noexcept = default;

void ArrayListFieldWidget::refreshImpl()
{
    while (!m_elements.empty()) {
        assert(m_elements.back() != nullptr);
        delete m_elements.back();
        m_elements.pop_back();
    }

    if (m_wrapper->hasFixedSize()) {
        m_wrapper->adjustFixedSize();
    }

    m_wrapper->refreshMembers();

    refreshInternal();
    addMissingFields();
    updatePrefixField();
    assert(m_elements.size() == m_wrapper->size());
}

void ArrayListFieldWidget::editEnabledUpdatedImpl()
{
    for (auto* elem : m_elements) {
        elem->setEditEnabled(isEditEnabled());
    }
    updateUi();
}

void ArrayListFieldWidget::updatePropertiesImpl(const QVariantMap& props)
{
    property::field::ArrayList arrayListProps(props);
    m_ui.m_prefixNameLabel->setText(arrayListProps.prefixName());
    m_prefixVisible = arrayListProps.isPrefixVisible();
    m_appendIndexToElementName = arrayListProps.isIndexAppendedToElementName();
    updatePrefixField();
    auto& elementsProps = arrayListProps.elements();

    m_elemProperties.clear();
    m_elemProperties.reserve(static_cast<std::size_t>(elementsProps.size()));
    m_elemProperties.assign(elementsProps.begin(), elementsProps.end());

    if (m_elemProperties.empty()) {
        return;
    }

    std::size_t idx = 0;
    for (std::size_t elemIdx = 0U; elemIdx < m_elements.size(); ++elemIdx) {
        auto* elem = m_elements[elemIdx];
        if (m_appendIndexToElementName) {
            elem->setNameSuffix(QString(" %1").arg(elemIdx));
        }

        elem->updateProperties(m_elemProperties[idx]);
        idx = ((idx + 1) % m_elemProperties.size());
    }
}

void ArrayListFieldWidget::dataFieldUpdated()
{
    if (!m_wrapper->canWrite()) {
        auto senderIter = std::find(m_elements.begin(), m_elements.end(), qobject_cast<ArrayListElementWidget*>(sender()));
        assert(senderIter != m_elements.end());
        auto idx = static_cast<unsigned>(std::distance(m_elements.begin(), senderIter));
        auto& memWrappers = m_wrapper->getMembers();
        assert(idx < memWrappers.size());
        auto& memWrapPtr = memWrappers[idx];
        if (!memWrapPtr->canWrite()) {
            memWrapPtr->reset();
            assert(memWrapPtr->canWrite());
            (*senderIter)->refresh();
        }

        if (!m_wrapper->canWrite()) {
            memWrapPtr->reset();
            assert(memWrapPtr->canWrite());
            (*senderIter)->refresh();
        }
    }

    refreshInternal();
    updatePrefixField();
    emitFieldUpdated();
}

void ArrayListFieldWidget::addNewField()
{
    m_wrapper->addField();
    refreshImpl();
    assert(m_elements.size() == m_wrapper->size());
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

    m_wrapper->removeField(idx);

    refreshImpl();

    assert(m_elements.size() == m_wrapper->size());
    assert(m_elements.size() == static_cast<unsigned>(m_ui.m_membersLayout->count()));

    emitFieldUpdated();
}

void ArrayListFieldWidget::addDataField(FieldWidget* dataFieldWidget)
{
    if (m_appendIndexToElementName) {
        dataFieldWidget->setNameSuffix(QString(" %1").arg(m_elements.size()));
    }

    auto* wrapperWidget = new ArrayListElementWidget(dataFieldWidget);
    wrapperWidget->setEditEnabled(isEditEnabled());
    wrapperWidget->setDeletable(!m_wrapper->hasFixedSize());

    if (!m_elemProperties.empty()) {
        auto elemPropsIdx = m_elements.size() % m_elemProperties.size();
        assert(elemPropsIdx < m_elemProperties.size());
        auto& elemProps = m_elemProperties[elemPropsIdx];
        wrapperWidget->updateProperties(elemProps);
    }

    connect(
        wrapperWidget, SIGNAL(sigFieldUpdated()),
        this, SLOT(dataFieldUpdated()));

    connect(
        wrapperWidget, SIGNAL(sigRemoveRequested()),
        this, SLOT(removeField()));

    m_elements.push_back(wrapperWidget);
    m_ui.m_membersLayout->addWidget(wrapperWidget);
}

void ArrayListFieldWidget::refreshInternal()
{
    assert(m_wrapper->canWrite());
    assert(m_ui.m_serValuePlainTextEdit != nullptr);
    updateSerValue(*m_ui.m_serValuePlainTextEdit, *m_wrapper);

    bool valid = m_wrapper->valid();
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValuePlainTextEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void ArrayListFieldWidget::updateUi()
{
    bool addButtonVisible = isEditEnabled() && (!m_wrapper->hasFixedSize());
    m_ui.m_addSepLine->setVisible(addButtonVisible);
    m_ui.m_addFieldPushButton->setVisible(addButtonVisible);
    updatePrefixField();
}

void ArrayListFieldWidget::addMissingFields()
{
    if (!m_createMissingDataFieldsCallback) {
        [[maybe_unused]] static constexpr bool Callback_should_exist = false;
        assert(Callback_should_exist);          
        return;
    }

    assert(m_elements.empty());
    auto fieldWidgets = m_createMissingDataFieldsCallback(*m_wrapper);
    for (auto& fieldWidgetPtr : fieldWidgets) {
        addDataField(fieldWidgetPtr.release());
    }

    assert(m_elements.size() == m_wrapper->size());
    assert(m_elements.size() == static_cast<unsigned>(m_ui.m_membersLayout->count()));
}

void ArrayListFieldWidget::updatePrefixField()
{
    if (!m_prefixVisible) {
        m_ui.m_prefixFieldWidget->hide();
        return;
    }

    auto info = m_wrapper->getPrefixFieldInfo();
    m_ui.m_prefixValueSpinBox->setValue(info.first);
    QString serText;
    for (auto byte : info.second) {
        serText.append(QString("%1").arg(static_cast<unsigned>(byte), 2, 16, QChar('0')));
    }
    m_ui.m_prefixSerValueLineEdit->setText(serText);
    m_ui.m_prefixFieldWidget->show();
}

}  // namespace cc_tools_qt



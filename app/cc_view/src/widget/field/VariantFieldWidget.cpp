//
// Copyright 2017 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "VariantFieldWidget.h"

#include <cassert>

namespace cc_tools_qt
{

namespace
{

const QString InvalidMemberComboText("???");
const int MemberNamesStartIndex = 2;

} // namespace

VariantFieldWidget::VariantFieldWidget(
    FieldPtr&& fieldPtr,
    CreateMemberFieldWidgetFunc&& func,
    QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr)),
    m_createFunc(std::move(func))
{
    m_ui.setupUi(this);
    setNameLabelWidget(m_ui.m_nameLabel);
    setValueWidget(m_ui.m_valueWidget);
    setSeparatorWidget(m_ui.m_sepLine);
    setSerialisedValueWidget(m_ui.m_serValueWidget);

    m_ui.m_idxSpinBox->setMaximum(m_fieldPtr->getMembersCount() - 1);
    fillMemberCombo();
    updateIndexValue();
    updateMemberCombo();

    commonConstruct();

    connect(
        m_ui.m_idxSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &VariantFieldWidget::indexUpdated);

    connect(
        m_ui.m_memberComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &VariantFieldWidget::memberComboUpdated);
}

VariantFieldWidget::~VariantFieldWidget() noexcept = default;

void VariantFieldWidget::setMemberField(FieldWidget* memberFieldWidget)
{
    assert(memberFieldWidget != nullptr);
    if (m_member != nullptr) {
        delete m_member;
    }
    m_member = memberFieldWidget;
    m_ui.m_membersLayout->addWidget(m_member);

    assert(m_ui.m_membersLayout->count() == 1);

    refreshInternal();

    connect(
        m_member, &FieldWidget::sigFieldUpdated,
        this, &VariantFieldWidget::memberFieldUpdated);
}

ToolsField& VariantFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void VariantFieldWidget::refreshImpl()
{
    refreshInternal();
    refreshMember();
}

void VariantFieldWidget::editEnabledUpdatedImpl()
{
    bool readOnly = !isEditEnabled();

    if (m_member != nullptr) {
        m_member->setEditEnabled(!readOnly);
    }

    updateIndexDisplay();
}

void VariantFieldWidget::memberFieldUpdated()
{
    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->reset();
        assert(m_fieldPtr->canWrite());
    }

    refreshInternal();
    emitFieldUpdated();
}

void VariantFieldWidget::indexUpdated(int value)
{
    assert(isEditEnabled());
    if (value == m_fieldPtr->getCurrentIndex()) {
        return;
    }

    destroyMemberWidget();

    if (0 <= value) {
        m_fieldPtr->setCurrentIndex(value);
        m_fieldPtr->updateCurrent();
        createMemberWidget();
    }

    updateMemberCombo();
    refresh();
    emitFieldUpdated();
}

void VariantFieldWidget::memberComboUpdated(int value)
{
    if (!isEditEnabled()) {
        updateMemberCombo();
        return;
    }

    if ((value < MemberNamesStartIndex) && (m_fieldPtr->getCurrentIndex() < 0)) {
        return;
    }

    destroyMemberWidget();

    if (MemberNamesStartIndex <= value) {
        auto dataVar = m_ui.m_memberComboBox->itemData(value, Qt::UserRole);
        assert(dataVar.isValid() && dataVar.canConvert<int>());
        int memIdx = dataVar.value<int>();
        m_fieldPtr->setCurrentIndex(memIdx);
        m_fieldPtr->updateCurrent();
        createMemberWidget();
    }

    updateIndexValue();
    refresh();
    emitFieldUpdated();
}

void VariantFieldWidget::refreshInternal()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValuePlainTextEdit != nullptr);
    updateSerValue(*m_ui.m_serValuePlainTextEdit, *m_fieldPtr);

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_serValuePlainTextEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);
}

void VariantFieldWidget::refreshMember()
{
    if (m_member != nullptr) {
        m_member->refresh();
    }
}

void VariantFieldWidget::updateIndexDisplay()
{
    bool readOnly = !isEditEnabled();
    m_ui.m_idxSpinBox->setReadOnly(readOnly);

    if (readOnly) {
        m_ui.m_idxSpinBox->setButtonSymbols(QSpinBox::NoButtons);
    }
    else {
        m_ui.m_idxSpinBox->setButtonSymbols(QSpinBox::UpDownArrows);
    }

    m_ui.m_idxWidget->setHidden(readOnly);

    bool idxWidgetHidden = readOnly && (m_ui.m_nameLabel->isHidden());
    m_ui.m_infoWidget->setHidden(idxWidgetHidden);
}

void VariantFieldWidget::updateIndexValue()
{
    auto memIdx = m_fieldPtr->getCurrentIndex();
    m_ui.m_idxSpinBox->blockSignals(true);
    m_ui.m_idxSpinBox->setValue(memIdx);
    m_ui.m_idxSpinBox->blockSignals(false);
}

void VariantFieldWidget::updateMemberCombo()
{
    auto memIdx = m_fieldPtr->getCurrentIndex();
    m_ui.m_memberComboBox->blockSignals(true);
    bool foundValid = false;
    for (int comboIdx = MemberNamesStartIndex; comboIdx < m_ui.m_memberComboBox->count(); ++comboIdx) {
        auto dataVar = m_ui.m_memberComboBox->itemData(comboIdx, Qt::UserRole);
        if ((!dataVar.isValid()) || (!dataVar.canConvert<int>())) {
            continue;
        }

        auto storedIdx = dataVar.value<int>();
        if (storedIdx == memIdx) {
            m_ui.m_memberComboBox->setCurrentIndex(comboIdx);
            foundValid = true;
            break;
        }
    }

    if (!foundValid) {
        m_ui.m_memberComboBox->setCurrentIndex(0); // Set unknown
    }

    m_ui.m_memberComboBox->blockSignals(false);
}

void VariantFieldWidget::destroyMemberWidget()
{
    delete m_member;
    m_member = nullptr;
    m_fieldPtr->setCurrent(ToolsFieldPtr());
    m_fieldPtr->setCurrentIndex(-1);
}

void VariantFieldWidget::createMemberWidget()
{
    assert(m_fieldPtr->getCurrent());
    assert(m_createFunc);
    auto fieldWidget = m_createFunc(*m_fieldPtr->getCurrent());
    m_member = fieldWidget.release();
    m_ui.m_membersLayout->addWidget(m_member);

    connect(
        m_member, &FieldWidget::sigFieldUpdated,
        this, &VariantFieldWidget::memberFieldUpdated);
}

void VariantFieldWidget::fillMemberCombo()
{
    auto membersNames = m_fieldPtr->membersNames();
    using MemberInfosList = std::vector<std::pair<QString, int> >;
    MemberInfosList membersInfo;
    membersInfo.reserve(static_cast<unsigned>(membersNames.size()));
    for (auto idx = 0; idx < membersNames.size(); ++idx) {
        auto& memName = membersNames[idx];
        if (memName.isEmpty()) {
            continue;
        }

        membersInfo.push_back(std::make_pair(memName, idx));
    }

    std::sort(
        membersInfo.begin(), membersInfo.end(),
        [](MemberInfosList::const_reference elem1, MemberInfosList::const_reference elem2)
        {
            return elem1.first < elem2.first;
        });

    m_ui.m_memberComboBox->blockSignals(true);
    m_ui.m_memberComboBox->clear();
    m_ui.m_memberComboBox->addItem(InvalidMemberComboText, QVariant(-1));
    m_ui.m_memberComboBox->insertSeparator(1);

    for (auto& i : membersInfo) {
        m_ui.m_memberComboBox->addItem(i.first, QVariant(i.second));
    }
    m_ui.m_memberComboBox->blockSignals(false);
}

}  // namespace cc_tools_qt



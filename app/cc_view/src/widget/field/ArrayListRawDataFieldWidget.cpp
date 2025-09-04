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

#include "ArrayListRawDataFieldWidget.h"

#include <algorithm>
#include <cassert>
#include <limits>

namespace cc_tools_qt
{

ArrayListRawDataFieldWidget::ArrayListRawDataFieldWidget(
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

    commonConstruct();

    refresh();

    connect(
        m_ui.m_valuePlainTextEdit, &QPlainTextEdit::textChanged,
        this, &ArrayListRawDataFieldWidget::valueChanged);

    connect(
        m_ui.m_showAllPushButton, &QPushButton::clicked,
        this, &ArrayListRawDataFieldWidget::showAllPressed);
}

ArrayListRawDataFieldWidget::~ArrayListRawDataFieldWidget() noexcept = default;

ToolsField& ArrayListRawDataFieldWidget::fieldImpl()
{
    assert(m_fieldPtr);
    return *m_fieldPtr;
}

void ArrayListRawDataFieldWidget::refreshImpl()
{
    assert(m_fieldPtr->canWrite());
    assert(m_ui.m_serValuePlainTextEdit != nullptr);
    updateSerValue(*m_ui.m_serValuePlainTextEdit, *m_fieldPtr);

    auto value = m_fieldPtr->getValue();
    bool valueUpdateNeeded = false;
    do {
        auto str = m_ui.m_valuePlainTextEdit->toPlainText().toLower();
        if (str == value) {
            break;
        }

        if (((str.size() + 1) == value.size()) &&
            (value[value.size() - 1] == QChar('0')) &&
            (str == value.left(value.size() - 1))) {
            break;
        }

        valueUpdateNeeded = true;
    } while (false);

    if (valueUpdateNeeded) {
        auto curs = m_ui.m_valuePlainTextEdit->textCursor();
        auto newPosition = std::min(curs.position(), static_cast<int>(value.size()));
        assert(m_ui.m_valuePlainTextEdit != nullptr);
        m_ui.m_valuePlainTextEdit->setPlainText(value);
        curs.setPosition(newPosition);
        m_ui.m_valuePlainTextEdit->setTextCursor(curs);
    }

    bool valid = m_fieldPtr->valid();
    setValidityStyleSheet(*m_ui.m_nameLabel, valid);
    setValidityStyleSheet(*m_ui.m_serFrontLabel, valid);
    setValidityStyleSheet(*m_ui.m_valuePlainTextEdit, valid);
    setValidityStyleSheet(*m_ui.m_serValuePlainTextEdit, valid);
    setValidityStyleSheet(*m_ui.m_serBackLabel, valid);

    m_ui.m_showAllWidget->setVisible(m_fieldPtr->isTruncated());
}

void ArrayListRawDataFieldWidget::editEnabledUpdatedImpl()
{
    bool readonly = !isEditEnabled();
    m_ui.m_valuePlainTextEdit->setReadOnly(readonly);
    if (readonly) {
        return;
    }

    bool refreshNeeded = m_fieldPtr->isTruncated();
    m_fieldPtr->setForcedShowAll(true);
    if (refreshNeeded) {
        refresh();
    }
}

void ArrayListRawDataFieldWidget::valueChanged()
{
    auto str = m_ui.m_valuePlainTextEdit->toPlainText();

    auto maxLen = std::numeric_limits<int>::max();
    if (m_fieldPtr->maxSize() < (maxLen / 2)) {
        maxLen = m_fieldPtr->maxSize() * 2;
    }
    auto minLen = m_fieldPtr->minSize() * 2;

    assert(0 <= maxLen);
    assert(0 <= minLen);
    assert(minLen <= maxLen);

    if (maxLen < str.size()) {
        str.resize(maxLen);
    }

    while (str.size() < minLen) {
        str.append('0');
    }

    auto oldValue = m_fieldPtr->getValue();
    m_fieldPtr->setValue(str);
    if (!m_fieldPtr->canWrite()) {
        m_fieldPtr->setValue(oldValue);
    }

    m_fieldPtr->setForcedShowAll(true);
    refresh();
    emitFieldUpdated();
}

void ArrayListRawDataFieldWidget::showAllPressed()
{
    m_fieldPtr->setForcedShowAll(true);
    refresh();
}

}  // namespace cc_tools_qt



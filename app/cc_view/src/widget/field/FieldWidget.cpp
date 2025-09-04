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


#include "FieldWidget.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpinBox>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>

namespace cc_tools_qt
{

namespace
{

void updateValidityStyle(
    QWidget& widget,
    bool valid,
    const QString& invalidStylesheet)
{
    static const auto DefaultStylesheet = QWidget().styleSheet();
    auto* stylesheet = &DefaultStylesheet;
    if (!valid) {
        stylesheet = &invalidStylesheet;
    }

    widget.setStyleSheet(*stylesheet);
}

}  // namespace

FieldWidget::FieldWidget(QWidget* parentObj)
  : Base(parentObj)
{
}

void FieldWidget::setNameSuffix(const QString& value)
{
    m_nameSuffix = value;
    performNameLabelUpdate();
    nameSuffixUpdatedImpl();
}

void FieldWidget::refresh()
{
    refreshImpl();
}

void FieldWidget::setEditEnabled(bool enabled)
{
    m_editEnabled = enabled;
    editEnabledUpdatedImpl();
}

void FieldWidget::emitFieldUpdated()
{
    emit sigFieldUpdated();
}

bool FieldWidget::isEditEnabled() const
{
    return m_editEnabled && (!m_forcedReadOnly);
}

void FieldWidget::setValidityStyleSheet(QLabel& widget, bool valid)
{
    static const QString InvalidStylesheet("QLabel { color: red }");
    updateValidityStyle(widget, valid, InvalidStylesheet);
}

void FieldWidget::setValidityStyleSheet(QLineEdit& widget, bool valid)
{
    static const QString InvalidStylesheet("QLineEdit { color: red }");
    updateValidityStyle(widget, valid, InvalidStylesheet);
}

void FieldWidget::setValidityStyleSheet(QPlainTextEdit& widget, bool valid)
{
    static const QString InvalidStylesheet("QPlainTextEdit { color: red }");
    updateValidityStyle(widget, valid, InvalidStylesheet);
}

void FieldWidget::setSerialisedInputMask(
    QLineEdit& line,
    int minWidth,
    int maxWidth)
{
    auto maskMinWidth = static_cast<std::size_t>(minWidth);
    auto maskMaxWidth = static_cast<std::size_t>(maxWidth);
    assert(maskMinWidth <= maskMaxWidth);
    QString mask;
    mask.reserve(static_cast<int>(maskMaxWidth));
    std::fill_n(std::back_inserter(mask), maskMinWidth, 'H');
    std::fill_n(std::back_inserter(mask), maskMaxWidth - maskMinWidth, 'h');
    line.setInputMask(mask);
}

void FieldWidget::setSerialisedInputMask(
    QLineEdit& line,
    int width)
{
    setSerialisedInputMask(line, width, width);
}

void FieldWidget::updateValue(QLineEdit& line, const QString& value)
{
    if (line.text() == value) {
        return;
    }

    auto cursorPos = std::min(static_cast<int>(value.size()), line.cursorPosition());
    line.setText(value);
    line.setCursorPosition(cursorPos);
}

void FieldWidget::updateSerValue(
    QPlainTextEdit& text,
    const ToolsField& field)
{
    QString serValueStr;
    auto serValue = field.getSerialisedValue();

    for (auto byte : serValue) {
        if (!serValueStr.isEmpty()) {
            serValueStr.append(QChar(' '));
        }
        serValueStr.append(QString("%1").arg(byte, 2, 16, QChar('0')));
    }

    text.setPlainText(serValueStr);
}

void FieldWidget::commonConstruct()
{
    performNameLabelUpdate();
    performSerVisibilityUpdate();
    performReadOnlyUpdate();
}

void FieldWidget::editEnabledUpdatedImpl()
{
}

void FieldWidget::nameSuffixUpdatedImpl()
{
}

void FieldWidget::performNameLabelUpdate()
{
    if (m_nameLabel == nullptr) {
        return;
    }

    auto& f = fieldImpl();

    QString str = f.name();
    if (str.isEmpty()) {
        m_nameLabel->hide();
        return;
    }

    if (!m_nameSuffix.isEmpty()) {
        str.append(m_nameSuffix);
    }

    str.append(':');
    m_nameLabel->setText(str);
    m_nameLabel->show();
}

void FieldWidget::performSerVisibilityUpdate()
{
    auto setWidgetHiddenFunc =
        [](QWidget* widget, bool hidden)
        {
            if (widget != nullptr) {
                widget->setHidden(hidden);
            }
        };
            
    auto& f = fieldImpl();
    bool serHidden = f.isHiddenSerialization();
    setWidgetHiddenFunc(m_sepWidget, serHidden);
    setWidgetHiddenFunc(m_serValueWidget, serHidden);    
}

void FieldWidget::performReadOnlyUpdate()
{
    auto readOnly = fieldImpl().isReadOnly();
    if (m_forcedReadOnly != readOnly) {
        m_forcedReadOnly = readOnly;
        editEnabledUpdatedImpl();
    }
}

}  // namespace cc_tools_qt


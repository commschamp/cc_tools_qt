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


#pragma once

#include <cassert>
#include <memory>
#include <functional>

#include <QtCore/QVariantMap>
#include <QtWidgets/QWidget>

#include "cc_tools_qt/ToolsField.h"

class QLineEdit;
class QLabel;
class QPlainTextEdit;
class QSpinBox;

namespace cc_tools_qt
{

class FieldWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Base;
public:
    FieldWidget(QWidget* parentObj = nullptr);
    ~FieldWidget() noexcept = default;

    void setNameSuffix(const QString& value);

    const QString& getNameSuffix() const
    {
        return m_nameSuffix;
    }

    ToolsField& field()
    {
        return fieldImpl();
    }

public slots:
    void refresh();
    void setEditEnabled(bool enabled);

signals:
    void sigFieldUpdated();

protected:
    void emitFieldUpdated();
    bool isEditEnabled() const;

    static void setValidityStyleSheet(QLabel& widget, bool valid);
    static void setValidityStyleSheet(QLineEdit& widget, bool valid);
    static void setValidityStyleSheet(QPlainTextEdit& widget, bool valid);
    static void setSerialisedInputMask(QLineEdit& line, int minWidth, int maxWidth);
    static void setSerialisedInputMask(QLineEdit& line, int width);
    static void updateValue(QLineEdit& line, const QString& value);
    static void updateSerValue(QPlainTextEdit& text, const ToolsField& field);

    using PostRefreshFunc = std::function<void ()>;
    template <typename TField>
    void handleNumericSerialisedValueUpdate(
        const QString& value,
        TField& field,
        PostRefreshFunc&& postRefreshFunc = PostRefreshFunc())
    {
        assert(isEditEnabled());
        do {
            if ((static_cast<std::size_t>(value.size()) & 0x1U) == 0) {
               field.setSerialisedString(value);
               break;
            }

            QString valueCpy(value);
            valueCpy.append(QChar('0'));
            field.setSerialisedString(valueCpy);
        } while (false);
        refresh();
        if (postRefreshFunc) {
            postRefreshFunc();
        }
        emitFieldUpdated();
    }

    void setNameLabelWidget(QLabel* widget)
    {
        m_nameLabel = widget;
    }

    void setValueWidget(QWidget* widget)
    {
        m_valueWidget = widget;
    }

    void setSeparatorWidget(QWidget* widget)
    {
        m_sepWidget = widget;
    }

    void setSerialisedValueWidget(QWidget* widget)
    {
        m_serValueWidget = widget;
    }

    void commonConstruct();

    virtual ToolsField& fieldImpl() = 0;
    virtual void refreshImpl() = 0;
    virtual void editEnabledUpdatedImpl();
    virtual void nameSuffixUpdatedImpl();

private:
    void performNameLabelUpdate();
    void performSerVisibilityUpdate();
    void performReadOnlyUpdate();
    
    bool m_forcedReadOnly = false;
    bool m_editEnabled = true;
    QLabel* m_nameLabel = nullptr;
    QWidget* m_valueWidget = nullptr;
    QWidget* m_sepWidget = nullptr;
    QWidget* m_serValueWidget = nullptr;
    QString m_nameSuffix;
};

typedef std::unique_ptr<FieldWidget> FieldWidgetPtr;

}  // namespace cc_tools_qt





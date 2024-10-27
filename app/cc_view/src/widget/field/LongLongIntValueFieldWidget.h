//
// Copyright 2017 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/field/ToolsIntField.h"
#include "FieldWidget.h"

#include "ui_LongLongIntValueFieldWidget.h"

namespace cc_tools_qt
{

class SpecialValueWidget;
class LongLongIntValueFieldWidget : public FieldWidget
{
    Q_OBJECT
    typedef FieldWidget Base;
public:
    using FieldPtr = field::ToolsIntFieldPtr;

    explicit LongLongIntValueFieldWidget(
        FieldPtr fieldPtr,
        QWidget* parentObj = nullptr);

    ~LongLongIntValueFieldWidget() noexcept;

protected:
    virtual ToolsField& fieldImpl() override;
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;

private slots:
    void serialisedValueUpdated(const QString& value);
    void valueUpdated(const QString& value);
    void specialSelected(long long value);

private:
    using SpecialInfo = QPair<QString, long long>;
    using SpecialsList = QList<SpecialInfo>;
    using FieldType = FieldPtr::element_type;
    using UnderlyingType = FieldType::UnderlyingType;
    typedef long long int DisplayedType;
    static DisplayedType getDisplayedValue(const QString& value);
    bool createSpecialsWidget(const SpecialsList& specials);

    Ui::LongLongIntValueFieldWidget m_ui;
    FieldPtr m_fieldPtr;
    SpecialValueWidget* m_specialsWidget = nullptr;
};


}  // namespace cc_tools_qt



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


#pragma once

#include "cc_tools_qt/field_wrapper/IntValueWrapper.h"
#include "FieldWidget.h"

#include "ui_ShortIntValueFieldWidget.h"


namespace cc_tools_qt
{

class SpecialValueWidget;
class ShortIntValueFieldWidget : public FieldWidget
{
    Q_OBJECT
    typedef FieldWidget Base;
public:
    using WrapperPtr = field_wrapper::IntValueWrapperPtr;

    explicit ShortIntValueFieldWidget(
        WrapperPtr wrapper,
        QWidget* parentObj = nullptr);

    ~ShortIntValueFieldWidget() noexcept;

protected:
    virtual void refreshImpl() override;
    virtual void editEnabledUpdatedImpl() override;
    virtual void updatePropertiesImpl(const QVariantMap& props) override;

private slots:
    void serialisedValueUpdated(const QString& value);
    void valueUpdated(int value);
    void specialSelected(long long value);

private:
    using SpecialInfo = QPair<QString, long long>;
    using SpecialsList = QList<SpecialInfo>;

    using WrapperType = WrapperPtr::element_type;
    using UnderlyingType = WrapperType::UnderlyingType;
    int adjustDisplayedToReal(int val);
    int adjustRealToDisplayed(int val);
    bool createSpecialsWidget(const SpecialsList& specials);

    Ui::ShortIntValueFieldWidget m_ui;
    WrapperPtr m_wrapper;
    int m_offset = 0;
    SpecialValueWidget* m_specialsWidget = nullptr;
};


}  // namespace cc_tools_qt



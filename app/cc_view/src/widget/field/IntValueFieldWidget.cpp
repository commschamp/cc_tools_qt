//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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


#include "IntValueFieldWidget.h"

#include <cassert>

#include <QtWidgets/QVBoxLayout>

#include "cc_tools_qt/property/field.h"

#include "ShortIntValueFieldWidget.h"
#include "LongIntValueFieldWidget.h"
#include "LongLongIntValueFieldWidget.h"
#include "ScaledIntValueFieldWidget.h"

namespace cc_tools_qt
{

IntValueFieldWidget::IntValueFieldWidget(FieldPtr fieldPtr, QWidget* parentObj)
  : Base(parentObj),
    m_fieldPtr(std::move(fieldPtr))
{
    assert(m_fieldPtr);
    assert(!m_childWidget);
    do {
        if (m_fieldPtr->hasScaledDecimals()) {
            m_childWidget.reset(new ScaledIntValueFieldWidget(std::move(m_fieldPtr)));
            break;
        }

        std::size_t valTypeSize = m_fieldPtr->valueTypeSize();
        bool isSigned = m_fieldPtr->isSigned();
        if ((valTypeSize < sizeof(int)) ||
             ((valTypeSize == sizeof(int) && isSigned))) {
            m_childWidget.reset(new ShortIntValueFieldWidget(std::move(m_fieldPtr)));
            break;
        }

        if (valTypeSize <= sizeof(unsigned)) {
            m_childWidget.reset(new LongIntValueFieldWidget(std::move(m_fieldPtr)));
            break;
        }

        if ((valTypeSize < sizeof(long long int)) ||
             ((valTypeSize == sizeof(long long int) && isSigned))) {
            m_childWidget.reset(new LongLongIntValueFieldWidget(std::move(m_fieldPtr)));
            break;
        }

        [[maybe_unused]] static constexpr bool The_handling_of_long_long_types_is_not_implemented_yet = false;
        assert(The_handling_of_long_long_types_is_not_implemented_yet);  
        return;
    } while (false);

    m_childWidget->setNameSuffix(getNameSuffix());
    assert(m_childWidget);
    auto* childLayout = new QVBoxLayout();
    childLayout->addWidget(m_childWidget.get());
    childLayout->setContentsMargins(0, 0, 0, 0);
    childLayout->setSpacing(0);
    setLayout(childLayout);
    m_childWidget->setEditEnabled(isEditEnabled());

    connect(
        m_childWidget.get(), SIGNAL(sigFieldUpdated()),
        this, SIGNAL(sigFieldUpdated()));   
}

IntValueFieldWidget::~IntValueFieldWidget() noexcept
{
    m_childWidget.release();
}

ToolsField& IntValueFieldWidget::fieldImpl()
{
    assert(m_childWidget != nullptr);
    return m_childWidget->field();
}

void IntValueFieldWidget::refreshImpl()
{
    assert((!m_fieldPtr) || (m_fieldPtr->canWrite()));
    if (m_childWidget) {
        m_childWidget->refresh();
    }
}

void IntValueFieldWidget::editEnabledUpdatedImpl()
{
    if (m_childWidget) {
        m_childWidget->setEditEnabled(isEditEnabled());
    }
}

void IntValueFieldWidget::nameSuffixUpdatedImpl()
{
    if (m_childWidget) {
        m_childWidget->setNameSuffix(getNameSuffix());
    }
}

}  // namespace cc_tools_qt


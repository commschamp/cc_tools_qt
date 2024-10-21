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

#include "cc_tools_qt/ToolsField.h"

#include <QtCore/QString>
#include <QtCore/QList>

#include <functional>
#include <memory>
#include <vector>

namespace cc_tools_qt
{

namespace field
{

class CC_API ToolsVariantField : public ToolsField
{
    using Base = ToolsField;
public:
    using ActPtr = std::unique_ptr<ToolsVariantField>;

    using MemberCreateCallbackFunc = std::function<ToolsFieldPtr ()>;

    ToolsVariantField();
    ToolsVariantField(const ToolsVariantField&) =delete;
    ToolsVariantField& operator=(const ToolsVariantField&) =delete;

    virtual ~ToolsVariantField() noexcept;

    ToolsFieldPtr& getCurrent();

    const ToolsFieldPtr& getCurrent() const;

    void setCurrent(ToolsFieldPtr current);

    void updateCurrent();

    ActPtr clone();

    QStringList membersNames() const;

    int getCurrentIndex() const;

    void setCurrentIndex(int index);

    int getMembersCount() const;

    template <typename TFunc>
    void setMemberCreateCallback(TFunc&& func)
    {
        m_createMemberCb = std::forward<TFunc>(func);
    }

protected:
    virtual ActPtr cloneImpl() = 0;
    virtual QStringList membersNamesImpl() const = 0;
    virtual void dispatchImpl(field_wrapper::FieldWrapperHandler& handler);
    virtual int getCurrentIndexImpl() const = 0;
    virtual void setCurrentIndexImpl(int index) = 0;
    virtual int getMembersCountImpl() const = 0;

private:
    ToolsFieldPtr m_current;
    MemberCreateCallbackFunc m_createMemberCb;
};

using ToolsVariantFieldPtr = ToolsVariantField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt

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


#pragma once

#include "cc_tools_qt/ToolsField.h"

#include <QtCore/QString>

#include <memory>

namespace cc_tools_qt
{

namespace field
{

class CC_API ToolsRawDataField : public ToolsField
{
public:

    typedef std::unique_ptr<ToolsRawDataField> ActPtr;

    ToolsRawDataField();
    virtual ~ToolsRawDataField() noexcept;

    QString getValue() const;

    void setValue(const QString& val);

    int maxSize() const;

    int minSize() const;

    ActPtr clone();

    bool getForcedShowAll() const;
    void setForcedShowAll(bool val = true);
    bool isTruncated() const;

protected:
    virtual QString getValueImpl() const = 0;
    virtual void setValueImpl(const QString& val) = 0;
    virtual int maxSizeImpl() const = 0;
    virtual int minSizeImpl() const = 0;
    virtual ActPtr cloneImpl() = 0;

    void dispatchImpl(field_wrapper::FieldWrapperHandler& handler);

    static const std::size_t TruncateLength = 128;

private:
    bool m_forcedShowAll = false;    
};

using ToolsRawDataFieldPtr = ToolsRawDataField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt




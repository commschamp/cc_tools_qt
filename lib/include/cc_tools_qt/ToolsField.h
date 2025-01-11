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

#include "cc_tools_qt/ToolsApi.h"

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace cc_tools_qt
{

class ToolsFieldHandler;

/// @brief Field class.
class CC_TOOLS_API ToolsField
{
public:
    using Ptr = std::unique_ptr<ToolsField>;
    using SerialisedSeq = std::vector<std::uint8_t>;
    using Members = std::vector<Ptr>;

    ToolsField();
    virtual ~ToolsField() noexcept;
    
    ToolsField(const ToolsField&) = delete;
    ToolsField& operator=(const ToolsField&) = delete;

    const char* name() const;

    std::size_t length() const;

    int width() const;

    bool valid() const;

    SerialisedSeq getSerialisedValue() const;

    bool setSerialisedValue(const SerialisedSeq& value);

    QString getSerialisedString() const;

    bool setSerialisedString(const QString& str);

    void dispatch(ToolsFieldHandler& handler);

    Ptr clone();

    bool canWrite() const;

    void reset();

    Members& getMembers();
    const Members& getMembers() const;
    void setMembers(Members&& members);    

    void forceReadOnly();
    bool isReadOnly() const;

    void forceHiddenSerialization();
    bool isHiddenSerialization() const;    

protected:
    virtual const char* nameImpl() const = 0;
    virtual std::size_t lengthImpl() const = 0;
    virtual bool validImpl() const = 0;
    virtual bool isReadOnlyImpl() const = 0;
    virtual bool isHiddenSerializationImpl() const = 0;
    virtual SerialisedSeq getSerialisedValueImpl() const = 0;
    virtual bool setSerialisedValueImpl(const SerialisedSeq& value) = 0;
    virtual void dispatchImpl(ToolsFieldHandler& handler) = 0;
    virtual Ptr cloneImpl() = 0;
    virtual bool canWriteImpl() const = 0;
    virtual void resetImpl() = 0;
    virtual void membersUpdatedImpl();

private:
    Members m_members;
    QVariantMap m_props;
};

using ToolsFieldPtr = ToolsField::Ptr;

}  // namespace cc_tools_qt

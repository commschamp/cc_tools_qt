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

#include "cc_tools_qt/details/ToolsNumericFieldBase.h"

#include <QtCore/QList>
#include <QtCore/QPair>

#include <memory>

namespace cc_tools_qt
{

namespace field
{

class CC_TOOLS_API ToolsEnumField : public cc_tools_qt::details::ToolsNumericFieldBase<long long int>
{
    using Base = cc_tools_qt::details::ToolsNumericFieldBase<long long int>;
public:
    using UnderlyingType = Base::UnderlyingType;
    using ActPtr = std::unique_ptr<ToolsEnumField>;
    using ValueInfo = QPair<QString, long long>;
    using ValueInfosList = QList<ValueInfo>;

    ToolsEnumField();
    virtual ~ToolsEnumField() noexcept;

    const ValueInfosList& values() const;

    ActPtr actClone();

protected:
    virtual const ValueInfosList& valuesImpl() const = 0;

    virtual void dispatchImpl(ToolsFieldHandler& handler) override;
};

using ToolsEnumFieldPtr = std::unique_ptr<ToolsEnumField>;

}  // namespace field

}  // namespace cc_tools_qt

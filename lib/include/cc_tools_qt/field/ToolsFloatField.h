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

class CC_API ToolsFloatField : public details::ToolsNumericFieldBase<double>
{
    using Base = details::ToolsNumericFieldBase<double>;
public:

    using UnderlyingType = Base::UnderlyingType;
    using ActPtr = std::unique_ptr<ToolsFloatField>;
    using SpecialType = QPair<QString, UnderlyingType>;
    using SpecialsList = QList<SpecialType>;    

    ToolsFloatField();
    virtual ~ToolsFloatField() noexcept;

    ActPtr clone();
    bool isNan() const;
    void setNan();
    bool isInf() const;
    void setInf();
    bool isMinusInf() const;
    void setMinusInf();
    double getEpsilon() const;
    const SpecialsList& specials() const;

protected:
    virtual ActPtr cloneImpl() = 0;
    virtual bool isNanImpl() const = 0;
    virtual void setNanImpl() = 0;
    virtual bool isInfImpl() const = 0;
    virtual void setInfImpl() = 0;
    virtual bool isMinusInfImpl() const = 0;
    virtual void setMinusInfImpl() = 0;
    virtual double getEpsilonImpl() const = 0;
    virtual const SpecialsList& specialsImpl() const = 0;

    virtual void dispatchImpl(ToolsFieldHandler& handler) override;
};

using ToolsFloatFieldPtr = ToolsFloatField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt

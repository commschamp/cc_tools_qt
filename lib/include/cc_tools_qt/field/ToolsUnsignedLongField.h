//
// Copyright 2017 - 2025 (C). Alex Robenko. All rights reserved.
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

#include <cstdint>
#include <cassert>
#include <memory>


namespace cc_tools_qt
{

namespace field
{

class CC_TOOLS_API ToolsUnsignedLongField : public cc_tools_qt::details::ToolsNumericFieldBase<long long unsigned>
{
    using Base = cc_tools_qt::details::ToolsNumericFieldBase<long long unsigned>;

public:
    using UnderlyingType = Base::UnderlyingType;
    using ActPtr = std::unique_ptr<ToolsUnsignedLongField>;
    using SpecialType = QPair<QString, UnderlyingType>;
    using SpecialsList = QList<SpecialType>;      

    ToolsUnsignedLongField();
    
    virtual ~ToolsUnsignedLongField() noexcept;

    UnderlyingType minValue() const;

    UnderlyingType maxValue() const;

    double getScaled() const;

    void setScaled(double value);

    double scaleValue(UnderlyingType value) const;

    bool isSigned() const;

    std::size_t valueTypeSize() const;

    const SpecialsList& specials() const;

    bool hasScaledDecimals() const;
    int scaledDecimals() const;    

    UnderlyingType getDisplayValue() const;
    void setDisplayValue(UnderlyingType value);    

    ActPtr actClone();

protected:
    virtual UnderlyingType minValueImpl() const = 0;
    virtual UnderlyingType maxValueImpl() const = 0;
    virtual double getScaledImpl() const = 0;
    virtual void setScaledImpl(double value) = 0;
    virtual double scaleValueImpl(UnderlyingType value) const = 0;
    virtual bool isSignedImpl() const = 0;
    virtual std::size_t valueTypeSizeImpl() const = 0;
    virtual const SpecialsList& specialsImpl() const = 0;
    virtual int scaledDecimalsImpl() const = 0;
    virtual UnderlyingType getDisplayValueImpl() const = 0;
    virtual void setDisplayValueImpl(UnderlyingType value) = 0;        

    virtual void dispatchImpl(ToolsFieldHandler& handler) override;
};

using ToolsUnsignedLongFieldPtr = ToolsUnsignedLongField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt

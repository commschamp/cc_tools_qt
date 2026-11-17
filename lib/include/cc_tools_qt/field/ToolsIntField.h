//
// Copyright 2014 - 2026 (C). Alex Robenko. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later
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

class CC_TOOLS_API ToolsIntField : public details::ToolsNumericFieldBase<long long int>
{
    using Base = details::ToolsNumericFieldBase<long long int>;

public:
    using UnderlyingType = Base::UnderlyingType ;
    using ActPtr = std::unique_ptr<ToolsIntField>;
    using SpecialType = QPair<QString, UnderlyingType>;
    using SpecialsList = QList<SpecialType>;

    ToolsIntField();
    virtual ~ToolsIntField() noexcept;

    [[nodiscard]] UnderlyingType minValue() const;

    [[nodiscard]] UnderlyingType maxValue() const;

    [[nodiscard]] double getScaled() const;

    void setScaled(double value);

    [[nodiscard]] double scaleValue(UnderlyingType value) const;

    bool isSigned() const;

    [[nodiscard]] std::size_t valueTypeSize() const;

    [[nodiscard]] const SpecialsList& specials() const;

    [[nodiscard]] bool hasScaledDecimals() const;
    [[nodiscard]] int scaledDecimals() const;

    [[nodiscard]] UnderlyingType getDisplayValue() const;
    void setDisplayValue(UnderlyingType value);

    [[nodiscard]] ActPtr actClone();

protected:
    [[nodiscard]] virtual UnderlyingType minValueImpl() const = 0;
    [[nodiscard]] virtual UnderlyingType maxValueImpl() const = 0;
    [[nodiscard]] virtual double getScaledImpl() const = 0;
    virtual void setScaledImpl(double value) = 0;
    [[nodiscard]] virtual double scaleValueImpl(UnderlyingType value) const = 0;
    [[nodiscard]] virtual bool isSignedImpl() const = 0;
    [[nodiscard]] virtual std::size_t valueTypeSizeImpl() const = 0;
    [[nodiscard]] virtual const SpecialsList& specialsImpl() const = 0;
    [[nodiscard]] virtual int scaledDecimalsImpl() const = 0;
    [[nodiscard]] virtual UnderlyingType getDisplayValueImpl() const = 0;
    virtual void setDisplayValueImpl(UnderlyingType value) = 0;

    virtual void dispatchImpl(ToolsFieldHandler& handler) override;
};

using ToolsIntFieldPtr = ToolsIntField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt

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

#include <memory>

namespace cc_tools_qt
{

namespace field
{

class CC_API ToolsBitmaskField : public details::ToolsNumericFieldBase<unsigned long long>
{
public:
    typedef std::unique_ptr<ToolsBitmaskField> ActPtr;

    virtual ~ToolsBitmaskField() noexcept;

    bool bitValue(unsigned idx) const;

    void setBitValue(unsigned idx, bool value);

    unsigned bitIdxLimit() const;

    ActPtr clone();

protected:
    virtual bool bitValueImpl(unsigned idx) const = 0;
    virtual void setBitValueImpl(unsigned idx, bool value) = 0;
    virtual unsigned bitIdxLimitImpl() const = 0;
    virtual ActPtr cloneImpl() = 0;

    virtual void dispatchImpl(ToolsFieldHandler& handler) override;
};

using ToolsBitmaskFieldPtr = ToolsBitmaskField::ActPtr;

}  // namespace field

}  // namespace cc_tools_qt

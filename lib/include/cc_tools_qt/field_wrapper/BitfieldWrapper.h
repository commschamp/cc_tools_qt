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

#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>

#include "comms/field/Bitfield.h"
#include "cc_tools_qt/ToolsField.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

class CC_API BitfieldWrapper : public ToolsField
{
    using Base = ToolsField;
public:
    typedef std::vector<ToolsFieldPtr> Members;

    typedef unsigned long long UnderlyingType;
    typedef std::unique_ptr<BitfieldWrapper> ActPtr;

    BitfieldWrapper();
    BitfieldWrapper(const BitfieldWrapper&) = delete;
    BitfieldWrapper& operator=(const BitfieldWrapper&) = delete;

    virtual ~BitfieldWrapper() noexcept;
    
    Members& getMembers();

    const Members& getMembers() const;

    void setMembers(Members&& members);

    ActPtr clone();

protected:
    virtual ActPtr cloneImpl() = 0;

    void dispatchImpl(FieldWrapperHandler& handler);

private:
    Members m_members;
};

template <typename TField>
class BitfieldWrapperT : public ToolsFieldT<BitfieldWrapper, TField>
{
    using Base = ToolsFieldT<BitfieldWrapper, TField>;
    using Field = TField;
    static_assert(comms::field::isBitfield<Field>(), "Must be of Bitfield field type");

    using UnderlyingType = typename Base::UnderlyingType;
public:
    typedef typename Base::ActPtr ActPtr;

    explicit BitfieldWrapperT(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    BitfieldWrapperT(const BitfieldWrapperT&) = default;
    BitfieldWrapperT(BitfieldWrapperT&&) = default;
    virtual ~BitfieldWrapperT() noexcept = default;

    BitfieldWrapperT& operator=(const BitfieldWrapperT&) = delete;

protected:
    virtual ActPtr cloneImpl() override
    {
        return ActPtr(new BitfieldWrapperT<TField>(Base::field()));
    }

};

using BitfieldWrapperPtr = BitfieldWrapper::ActPtr;

template <typename TField>
BitfieldWrapperPtr
makeBitfieldWrapper(TField& field)
{
    return
        BitfieldWrapperPtr(
            new BitfieldWrapperT<TField>(field));
}

}  // namespace field_wrapper

}  // namespace cc_tools_qt

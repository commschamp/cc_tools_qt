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

#include <memory>
#include <cassert>

#include "comms/field/Optional.h"
#include "cc_tools_qt/ToolsField.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

class CC_API OptionalWrapper : public ToolsField
{
public:
    typedef std::unique_ptr<OptionalWrapper> ActPtr;
    typedef comms::field::OptionalMode Mode;

    OptionalWrapper();
    virtual ~OptionalWrapper() noexcept;

    Mode getMode() const;

    void setMode(Mode mode);

    bool hasFieldWrapper() const;

    ToolsField& getFieldWrapper();

    const ToolsField& getFieldWrapper() const;

    void setFieldWrapper(ToolsFieldPtr fieldWrapper);

    ActPtr clone();

protected:
    virtual Mode getModeImpl() const = 0;
    virtual void setModeImpl(Mode mode) = 0;
    virtual ActPtr cloneImpl() = 0;

    void dispatchImpl(FieldWrapperHandler& handler);

private:
    ToolsFieldPtr m_fieldWrapper;
};

template <typename TField>
class OptionalWrapperT : public ToolsFieldT<OptionalWrapper, TField>
{
    typedef ToolsFieldT<OptionalWrapper, TField> Base;
public:

    typedef typename Base::Mode Mode;
    typedef typename Base::ActPtr ActPtr;

    typedef TField Field;
    explicit OptionalWrapperT(Field& fieldRef)
      : Base(fieldRef)
    {
    }

protected:
    virtual Mode getModeImpl() const override
    {
        return Base::field().getMode();
    }

    virtual void setModeImpl(Mode mode) override
    {
        Base::field().setMode(mode);
    }

    virtual ActPtr cloneImpl() override
    {
        return ActPtr(new OptionalWrapperT<TField>(Base::field()));
    }
};

using OptionalWrapperPtr = OptionalWrapper::ActPtr;

template <typename TField>
OptionalWrapperPtr
makeOptionalWrapper(TField& field)
{
    return
        OptionalWrapperPtr(
            new OptionalWrapperT<TField>(field));
}


}  // namespace field_wrapper

}  // namespace cc_tools_qt



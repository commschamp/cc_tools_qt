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

#include "comms/field/Bundle.h"
#include "cc_tools_qt/ToolsField.h"

namespace cc_tools_qt
{

namespace field_wrapper
{

class CC_API BundleWrapper : public ToolsField
{
    using Base = ToolsField;
public:
    typedef std::vector<ToolsFieldPtr> Members;

    typedef std::unique_ptr<BundleWrapper> Ptr;

    BundleWrapper();
    BundleWrapper(const BundleWrapper&) =delete;
    BundleWrapper& operator=(const BundleWrapper&) =delete;

    virtual ~BundleWrapper() noexcept;

    Members& getMembers();

    const Members& getMembers() const;

    void setMembers(Members&& members);

    Ptr clone();

protected:
    virtual Ptr cloneImpl() = 0;

    void dispatchImpl(FieldWrapperHandler& handler);

private:
    Members m_members;
};

template <typename TField>
class BundleWrapperT : public ToolsFieldT<BundleWrapper, TField>
{
    using Base = ToolsFieldT<BundleWrapper, TField>;
    using Field = TField;
    static_assert(comms::field::isBundle<Field>(), "Must be of Bundle field type");

public:
    typedef typename Base::Ptr Ptr;

    explicit BundleWrapperT(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    BundleWrapperT(const BundleWrapperT&) = default;
    BundleWrapperT(BundleWrapperT&&) = default;
    virtual ~BundleWrapperT() noexcept = default;

    BundleWrapperT& operator=(const BundleWrapperT&) = delete;

protected:
    virtual Ptr cloneImpl() override
    {
        return Ptr(new BundleWrapperT(Base::field()));
    }
};

using BundleWrapperPtr = BundleWrapper::Ptr;

template <typename TField>
BundleWrapperPtr
makeBundleWrapper(TField& field)
{
    return
        BundleWrapperPtr(
            new BundleWrapperT<TField>(field));
}

}  // namespace field_wrapper

}  // namespace cc_tools_qt

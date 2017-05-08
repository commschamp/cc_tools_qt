//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
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

#include "comms_champion/field_wrapper/VariantWrapper.h"

#include "comms_champion/field_wrapper/FieldWrapperHandler.h"

namespace comms_champion
{

namespace field_wrapper
{

VariantWrapper::VariantWrapper() {}

VariantWrapper::~VariantWrapper() {}

FieldWrapper& VariantWrapper::getCurrent()
{
    return *m_current;
}

const FieldWrapper& VariantWrapper::getCurrent() const
{
    return *m_current;
}

void VariantWrapper::setCurrent(FieldWrapperPtr current)
{
    m_current = std::move(current);
} 

VariantWrapper::Ptr VariantWrapper::clone()
{
    auto curr = m_current->upClone();
    auto ptr = cloneImpl();
    ptr->setCurrent(std::move(curr));
    return ptr;
}

int VariantWrapper::getCurrentIndex() const
{
    return getCurrentIndexImpl();
}

void VariantWrapper::setCurrentIndex(int index)
{
    setCurrentIndexImpl(index);
}

void VariantWrapper::dispatchImpl(FieldWrapperHandler& handler)
{
    handler.handle(*this);
}

}  // namespace field_wrapper

}  // namespace comms_champion



//
// Copyright 2015 - 2018 (C). Alex Robenko. All rights reserved.
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

#include "BitmaskValues.h"

#include "cc_tools_qt/property/field.h"

#include <cassert>

namespace cc = cc_tools_qt;

namespace demo
{

namespace cc_plugin
{

namespace message
{

namespace
{

using ProtMsg = demo::cc_plugin::message::BitmaskValues::ProtMsg;

QVariantList createFieldsProperties()
{
    using Field1 = ProtMsg::Field_field1;
    QVariantList props;
    props.append(
        cc::property::field::ForField<Field1>()
            .add("bit0")
            .add("bit1")
            .add("bit2")
            .add("bit3")
            .add("bit4")
            .asMap());

    assert(
        cc::property::field::BitmaskValue(props.back())
            .bits().size() == Field1::BitIdx_numOfValues);

    using Field2 = ProtMsg::Field_field2;
    props.append(
        cc::property::field::ForField<Field2>()
            .add("bit0")
            .add(Field2::BitIdx_bit3, "bit3")
            .add(Field2::BitIdx_bit8, "bit8")
            .add("bit9")
            .asMap());
    assert(
        cc::property::field::BitmaskValue(props.back())
            .bits().size() == static_cast<int>(Field2::BitIdx_numOfValues));

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

BitmaskValues::BitmaskValues() = default;
BitmaskValues::~BitmaskValues() noexcept = default;

BitmaskValues& BitmaskValues::operator=(const BitmaskValues&) = default;
BitmaskValues& BitmaskValues::operator=(BitmaskValues&&) = default;

const QVariantList& BitmaskValues::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


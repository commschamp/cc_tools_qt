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

#include "EnumValues.h"

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

using ProtMsg = demo::cc_plugin::message::EnumValues::ProtMsg;

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(
        cc::property::field::ForField<ProtMsg::Field_field1>()
            .name("field1")
            .add("Value1")
            .add("Value2")
            .add("Value3")
            .asMap());
    assert(
        cc::property::field::EnumValue(props.back())
            .values().size() == static_cast<int>(ProtMsg::Field_field1::ValueType::NumOfValues));

    props.append(
        cc::property::field::ForField<ProtMsg::Field_field2>()
            .name("field2")
            .add("Value1", static_cast<int>(ProtMsg::Field_field2::ValueType::Value1))
            .add("Value2", static_cast<int>(ProtMsg::Field_field2::ValueType::Value2))
            .add("Value3", static_cast<int>(ProtMsg::Field_field2::ValueType::Value3))
            .add("Value4", static_cast<int>(ProtMsg::Field_field2::ValueType::Value4))
            .asMap());

    props.append(
        cc::property::field::ForField<ProtMsg::Field_field3>()
            .name("field3")
            .add("Value1", static_cast<int>(ProtMsg::Field_field3::ValueType::Value1))
            .add("Value2", static_cast<int>(ProtMsg::Field_field3::ValueType::Value2))
            .add("Value3", static_cast<int>(ProtMsg::Field_field3::ValueType::Value3))
            .add("Value4", static_cast<int>(ProtMsg::Field_field3::ValueType::Value4))
            .add("Value5", static_cast<int>(ProtMsg::Field_field3::ValueType::Value5))
            .asMap());

    props.append(
        cc::property::field::ForField<ProtMsg::Field_field4>()
            .name("field3")
            .add("Value1", static_cast<long long>(ProtMsg::Field_field4::ValueType::Value1))
            .add("Value2", static_cast<long long>(ProtMsg::Field_field4::ValueType::Value2))
            .add("Value3", static_cast<long long>(ProtMsg::Field_field4::ValueType::Value3))
            .asMap());

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

EnumValues::EnumValues() = default;
EnumValues::~EnumValues() noexcept = default;

EnumValues& EnumValues::operator=(const EnumValues&) = default;
EnumValues& EnumValues::operator=(EnumValues&&) = default;

const QVariantList& EnumValues::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


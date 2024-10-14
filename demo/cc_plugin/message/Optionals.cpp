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

#include "Optionals.h"

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

using ProtMsg = demo::cc_plugin::message::Optionals::ProtMsg;

QVariantMap createField1Properties()
{
    using Field = ProtMsg::Field_field1;
    cc::property::field::ForField<Field> props;
    props.add("enable_field2")
         .add("enable_field3");

    assert(props.bits().size() == Field::BitIdx_numOfValues);
    return props.asMap();
}

QVariantMap createField2Properties()
{
    return
        cc::property::field::ForField<ProtMsg::Field_field2>()
            .field(
                cc::property::field::ForField<ProtMsg::Field_field2::Field>()
                    .asMap())
            .uncheckable()
            .asMap();
}

QVariantMap createField3Properties()
{
    return
        cc::property::field::ForField<ProtMsg::Field_field3>()
            .field(
                cc::property::field::ForField<ProtMsg::Field_field3::Field>()
                    .asMap())
            .uncheckable()
            .asMap();
}

QVariantMap createField4Properties()
{
    return
        cc::property::field::ForField<ProtMsg::Field_field4>()
            .field(
                cc::property::field::ForField<ProtMsg::Field_field4::Field>()
                    .asMap())
            .uncheckable()
            .asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createField1Properties());
    props.append(createField2Properties());
    props.append(createField3Properties());
    props.append(createField4Properties());

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

Optionals::Optionals() = default;
Optionals::~Optionals() noexcept = default;

Optionals& Optionals::operator=(const Optionals&) = default;
Optionals& Optionals::operator=(Optionals&&) = default;

const QVariantList& Optionals::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo

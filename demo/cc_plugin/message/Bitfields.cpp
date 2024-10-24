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

#include "Bitfields.h"

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

using ProtMsg = demo::cc_plugin::message::Bitfields::ProtMsg;

QVariantMap createField1BitmaskProperties()
{
    using Field = ProtMsg::Field_field1::Field_member1;
    auto props = 
        cc::property::field::ForField<Field>()
        .serialisedHidden();
    return props.asMap();
}

QVariantMap createField1EnumProperties()
{
    using Field = ProtMsg::Field_field1::Field_member2;
    auto props = 
        cc::property::field::ForField<Field>()
            .serialisedHidden();

    return props.asMap();
}

QVariantMap createField1Int1Properties()
{
    using Field = ProtMsg::Field_field1::Field_member3;
    auto props = 
        cc::property::field::ForField<Field>()
            .serialisedHidden();

    return props.asMap();
}

QVariantMap createField1Int2Properties()
{
    using Field = ProtMsg::Field_field1::Field_member4;
    auto props = 
        cc::property::field::ForField<Field>()
            .serialisedHidden();

    return props.asMap();
}

QVariantMap createField1Properties()
{
    using Field = ProtMsg::Field_field1;
    auto props = 
        cc::property::field::ForField<Field>()
            .add(createField1BitmaskProperties())
            .add(createField1EnumProperties())
            .add(createField1Int1Properties())
            .add(createField1Int2Properties());

    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createField1Properties());

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

Bitfields::Bitfields() = default;
Bitfields::~Bitfields() noexcept = default;

Bitfields& Bitfields::operator=(const Bitfields&) = default;
Bitfields& Bitfields::operator=(Bitfields&&) = default;

const QVariantList& Bitfields::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


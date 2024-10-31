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

#include "Lists.h"

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

using ProtMsg = demo::cc_plugin::message::Lists::ProtMsg;

QVariantMap createField3Properties()
{
    using Field3 = ProtMsg::Field_field3;
    cc::property::field::ForField<Field3> props;
    return props.asMap();
}

QVariantMap createField4Properties()
{
    return
        cc::property::field::ForField<ProtMsg::Field_field4>()
            .asMap();
}

QVariantMap createField5Properties()
{
    return
        cc::property::field::ForField<ProtMsg::Field_field5>()
            .asMap();
}


QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc::property::field::ForField<ProtMsg::Field_field1>().asMap());
    props.append(cc::property::field::ForField<ProtMsg::Field_field2>().asMap());
    props.append(createField3Properties());
    props.append(createField4Properties());
    props.append(createField5Properties());

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

Lists::Lists() = default;
Lists::~Lists() noexcept = default;

Lists& Lists::operator=(const Lists&) = default;
Lists& Lists::operator=(Lists&&) = default;

const QVariantList& Lists::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


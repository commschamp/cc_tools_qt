//
// Copyright 2015 - 2016 (C). Alex Robenko. All rights reserved.
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

#include "DemoTransportMessage.h"

#include "cc_tools_qt/property/field.h"

#include <QtCore/QVariantMap>

#include <cassert>

namespace cc = cc_tools_qt;

namespace demo
{

namespace cc_plugin
{

namespace
{

using ProtMsg = DemoTransportMessage::ProtMsg;    

QVariantMap createMsgIdProperties()
{
    static const char* Names[] = {
        "IntValues",
        "EnumValues",
        "BitmaskValues",
        "Bitfields",
        "Strings",
        "Lists",
        "Optionals",
        "FloatValues",
        "Variants",
        "Bundles",
    };

    static const auto NamesCount = std::extent<decltype(Names)>::value;
    static_assert(NamesCount == demo::MsgId_NumOfValues, "Not all messages are added");

    cc::property::field::ForField<MsgIdField> props;

    QVariantList enumValues;
    for (auto name : Names) {
        props.add(name);
    }

    assert(props.values().size() == demo::MsgId_NumOfValues);
    return props.asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc::property::field::ForField<demo::SyncField>().asMap());
    props.append(
        cc::property::field::ForField<demo::LengthField>()
            .displayOffset(2)
            .asMap());
    props.append(createMsgIdProperties());
    props.append(cc::property::field::ForField<demo::VersionField>().asMap());
    props.append(cc::property::field::ForField<demo::DataField<> >().asMap());
    props.append(cc::property::field::ForField<demo::ChecksumField>().asMap());
    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

const QVariantList& DemoTransportMessage::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

qlonglong DemoTransportMessage::numericIdImpl() const 
{
    return static_cast<qlonglong>(msg().field_id().value());
}

}  // namespace cc_plugin

}  // namespace demo


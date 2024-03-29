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

#include "RawDataProtocolTransportMessage.h"

#include <cassert>

#include <QtCore/QVariantMap>

namespace cc = cc_tools_qt;

namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

namespace cc_plugin
{

namespace
{

typedef RawDataProtocolTransportMessage::Field FieldBase;
typedef raw_data_protocol::IdField<FieldBase> IdField;
typedef raw_data_protocol::DataField<> DataField;

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc::property::field::ForField<IdField>().hidden().asMap());
    props.append(cc::property::field::ForField<DataField>().name("Data").asMap());
    assert(props.size() == RawDataProtocolTransportMessage::FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const QVariantList& RawDataProtocolTransportMessage::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

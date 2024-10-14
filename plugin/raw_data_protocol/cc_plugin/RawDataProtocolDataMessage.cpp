//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "RawDataProtocolDataMessage.h"

#include "cc_tools_qt/property/field.h"

#include <cassert>

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

using ProtMsg = cc_tools_qt::plugin::raw_data_protocol::cc_plugin::RawDataProtocolDataMessage::ProtMsg;

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc::property::field::ForField<ProtMsg::Field_data>().asMap());

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

const QVariantList& RawDataProtocolDataMessage::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

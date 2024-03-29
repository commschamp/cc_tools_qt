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

#include <cassert>

#include "RawDataProtocolDataMessage.h"

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

typedef raw_data_protocol::DataMessageFields<RawDataProtocolDataMessage::Field> DataMessageFields;

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc::property::field::ForField<DataMessageFields::data>().name("data").asMap());

    assert(props.size() == RawDataProtocolDataMessage::FieldIdx_numOfValues);
    return props;
}

}  // namespace

RawDataProtocolDataMessage::RawDataProtocolDataMessage() = default;
RawDataProtocolDataMessage::~RawDataProtocolDataMessage() noexcept = default;

RawDataProtocolDataMessage& RawDataProtocolDataMessage::operator=(const RawDataProtocolDataMessage&) = default;
RawDataProtocolDataMessage& RawDataProtocolDataMessage::operator=(RawDataProtocolDataMessage&&) = default;

const char* RawDataProtocolDataMessage::nameImpl() const
{
    static const char* Str = "Raw Data";
    return Str;
}

const QVariantList& RawDataProtocolDataMessage::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt

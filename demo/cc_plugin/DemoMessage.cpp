//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
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

#include "DemoMessage.h"

#include <cassert>

namespace cc = cc_tools_qt;

namespace demo
{

namespace cc_plugin
{

namespace
{

static QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc::property::field::IntValue().name("Version").serialisedHidden().hiddenWhenReadOnly().asMap());

    assert(props.size() == demo::DemoMessage<>::TransportFieldIdx_numOfValues);
    return props;
}

} // namespace

DemoMessage::DemoMessage() = default;
DemoMessage::~DemoMessage() noexcept = default;

const QVariantList& DemoMessage::extraTransportFieldsPropertiesImpl() const
{
    if (getId() != demo::MsgId_Optionals) {
        return Base::extraTransportFieldsPropertiesImpl();
    }

    static const QVariantList Props = createFieldsProperties();
    return Props;
}

QString DemoMessage::idAsStringImpl() const
{
    return QString("0x%1").arg(getId(), 2, 16, QChar('0'));
}

} // namespace cc_plugin

} // namespace demo

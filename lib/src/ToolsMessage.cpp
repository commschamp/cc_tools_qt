//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsMessage.h"

#include "cc_tools_qt/ToolsStaticSingleton.h"

#include <QtCore/QVariant>

#include <cassert>

namespace cc_tools_qt
{

namespace
{

struct MetaTypesRegistrator
{
    MetaTypesRegistrator()
    {
        qRegisterMetaType<ToolsMessagePtr>();
        qRegisterMetaType<ToolsMessagesList>();
        qRegisterMetaType<ToolsMessage::DataSeq>();
        qRegisterMetaType<ToolsMessage::Type>();
    }
};

void registerMetaTypesIfNeeded()
{
    [[maybe_unused]] static const MetaTypesRegistrator Registrator;
}

} // namespace

ToolsMessage::~ToolsMessage() noexcept = default;

const char* ToolsMessage::name() const
{
    return nameImpl();
}

bool ToolsMessage::refreshMsg()
{
    return refreshMsgImpl();
}

QString ToolsMessage::idAsString() const
{
    return idAsStringImpl();
}

qlonglong ToolsMessage::numericId() const
{
    return numericIdImpl();
}

void ToolsMessage::reset()
{
    resetImpl();
}

bool ToolsMessage::assign(const ToolsMessage& other)
{
    return assignImpl(other);
}

bool ToolsMessage::isValid() const
{
    return isValidImpl();
}

ToolsMessage::DataSeq ToolsMessage::encodeData() const
{
    return encodeDataImpl();
}

bool ToolsMessage::decodeData(const DataSeq& data)
{
    return decodeDataImpl(data);
}

ToolsMessage::Ptr ToolsMessage::clone() const
{
    return cloneImpl();
}

void ToolsMessage::assignProtMessage(void* protMsg)
{
    assignProtMessageImpl(protMsg);
}

ToolsMessage::DataSeq ToolsMessage::encodeFramed(ToolsFrame& frame) const
{
    return encodeFramedImpl(frame);
}

ToolsMessage::FieldsList ToolsMessage::transportFields()
{
    auto fields = transportFieldsImpl();
    for (auto& f : fields) {
        f->forceHiddenSerialization();
    }
    return fields;
}

ToolsMessage::FieldsList ToolsMessage::payloadFields()
{
    return payloadFieldsImpl();
}

ToolsMessage::ToolsMessage()
{
    registerMetaTypesIfNeeded();
}

QString ToolsMessage::idAsStringImpl() const
{
    return QString("%1").arg(numericIdImpl());
}

}  // namespace cc_tools_qt


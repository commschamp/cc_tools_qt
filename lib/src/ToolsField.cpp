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


#include "cc_tools_qt/ToolsField.h"

#include <cassert>

namespace cc_tools_qt
{

ToolsField::ToolsField() = default;

ToolsField::~ToolsField() noexcept = default;

const char* ToolsField::name() const
{
    return nameImpl();
}

std::size_t ToolsField::length() const
{
    return lengthImpl();
}

int ToolsField::width() const
{
    return static_cast<int>(length()) * 2;
}

bool ToolsField::valid() const
{
    return validImpl();
}

ToolsField::SerialisedSeq ToolsField::getSerialisedValue() const
{
    return getSerialisedValueImpl();
}

bool ToolsField::setSerialisedValue(const SerialisedSeq& value)
{
    return setSerialisedValueImpl(value);
}

QString ToolsField::getSerialisedString() const
{
    auto seq = getSerialisedValue();
    QString str;
    for (auto& byte : seq) {
        str.append(QString("%1").arg(static_cast<unsigned>(byte), 2, 16, QChar('0')));
    }
    return str;
}

bool ToolsField::setSerialisedString(const QString& str)
{
    assert((str.size() & 0x1) == 0U);
    SerialisedSeq seq;
    seq.reserve(static_cast<std::size_t>(str.size() / 2));
    QString byteStr;
    for (auto ch : str) {
        byteStr.append(ch);
        if (byteStr.size() < 2) {
            continue;
        }

        bool ok = false;
        auto val = byteStr.toUInt(&ok, 16);
        if (ok) {
            seq.push_back(static_cast<SerialisedSeq::value_type>(val));
        }
        byteStr.clear();
    }

    return setSerialisedValue(seq);
}

void ToolsField::dispatch(field_wrapper::FieldWrapperHandler& handler)
{
    dispatchImpl(handler);
}

ToolsField::BasePtr ToolsField::upClone()
{
    return upCloneImpl();
}

bool ToolsField::canWrite() const
{
    return canWriteImpl();
}

void ToolsField::reset()
{
    return resetImpl();
}

}  // namespace cc_tools_qt



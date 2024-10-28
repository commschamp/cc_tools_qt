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
#include <vector>
#include <utility>

namespace cc_tools_qt
{

namespace 
{

const QString& readOnlyProp()
{
    static const QString Str("cc.read_only");
    return Str;
}

const QString& hiddenSerializationProp()
{
    static const QString Str("cc.hidden_ser");
    return Str;
}

} // namespace 
    

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

void ToolsField::dispatch(ToolsFieldHandler& handler)
{
    dispatchImpl(handler);
}

ToolsField::Ptr ToolsField::clone()
{
    Members clonedMembers;
    clonedMembers.reserve(m_members.size());
    for (auto& m : m_members) {
        clonedMembers.push_back(m->clone());
    }
    auto ptr = cloneImpl();
    ptr->setMembers(std::move(clonedMembers));
    ptr->m_props = m_props;
    return ptr;
}

bool ToolsField::canWrite() const
{
    return canWriteImpl();
}

void ToolsField::reset()
{
    return resetImpl();
}

ToolsField::Members& ToolsField::getMembers()
{
    return m_members;
}

const ToolsField::Members& ToolsField::getMembers() const
{
    return m_members;
}

void ToolsField::setMembers(Members&& members)
{
    m_members = std::move(members);
    for (auto& m : m_members) {
        using CheckFunc = bool (ToolsField::*)() const;
        using ForceFunc = void (ToolsField::*)();
        static const std::vector<std::pair<CheckFunc, ForceFunc>> List = {
            {&ToolsField::isReadOnly, &ToolsField::forceReadOnly},
        };

        assert(m);
        for (auto& funcs : List) {
            if ((m.get()->*funcs.first)()) {
                (m.get()->*funcs.second)();
            }
        }
    }
}

void ToolsField::forceReadOnly()
{
    m_props[readOnlyProp()] = true;
    for (auto& m : m_members) {
        assert(m);
        m->forceReadOnly();
    }
}

bool ToolsField::isReadOnly() const
{
    if (m_props.contains(readOnlyProp())) {
        return true;
    }

    return isReadOnlyImpl();
}

void ToolsField::forceHiddenSerialization()
{
    m_props[hiddenSerializationProp()] = true;
    for (auto& m : m_members) {
        assert(m);
        m->forceHiddenSerialization();
    }
}

bool ToolsField::isHiddenSerialization() const
{
    if (m_props.contains(hiddenSerializationProp())) {
        return true;
    }

    return isHiddenSerializationImpl();
}

}  // namespace cc_tools_qt



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


#pragma once


#include <cassert>
#include <tuple>

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QByteArray>

#include "comms/comms.h"
#include "property/field.h"
#include "ProtocolMessageBase.h"
#include "cc_tools_qt.h"

namespace cc_tools_qt
{

namespace details
{

template <typename TFieldBase>
struct ExtraInfoMessageData : public comms::field::String<TFieldBase>
{
    bool valid() const
    {
        using Base = comms::field::String<TFieldBase>;

        auto& val = Base::value();
        if (val.empty()) {
            return true;
        }

        auto doc = QJsonDocument::fromJson(QByteArray(val.c_str(), static_cast<int>(val.size())));
        return doc.isObject();
    }
};

template <typename TMsgBase>
class ExtraInfoMessageImpl : public
    comms::MessageBase<
        TMsgBase,
        comms::option::NoIdImpl,
        comms::option::FieldsImpl<std::tuple<ExtraInfoMessageData<typename TMsgBase::Field> > >,
        comms::option::MsgType<ExtraInfoMessageImpl<TMsgBase> >
    >
{

};

}  // namespace details

template <typename TMsgBase>
class ExtraInfoMessage : public
    ProtocolMessageBase<
        details::ExtraInfoMessageImpl<TMsgBase>,
        ExtraInfoMessage<TMsgBase>
    >
{
public:
    virtual ~ExtraInfoMessage() noexcept = default;

protected:
    virtual const char*
    nameImpl() const override
    {
        static const char* Str = "Generic Extra Info Message";
        return Str;
    }

    /// @brief Overriding virtual cc_tools_qt::Message::extraTransportFieldsPropertiesImpl()
    virtual const QVariantList&  extraTransportFieldsPropertiesImpl() const override
    {
        static const QVariantList List;
        return List;
    }

    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        static const QVariantList Props = createFieldsProperties();
        return Props;
    }

    virtual QString idAsStringImpl() const override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);         
        return QString();
    }

    virtual void resetImpl() override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }

    virtual bool assignImpl([[maybe_unused]] const cc_tools_qt::Message& other) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return false;
    }

private:

    static QVariantList createFieldsProperties()
    {
        QVariantList props;
        props.append(
            property::field::String().serialisedHidden().asMap());
        return props;
    }
};

}  // namespace cc_tools_qt




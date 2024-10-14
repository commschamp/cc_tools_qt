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


#pragma once

#include "cc_tools_qt/ToolsMessageBase.h"
#include "cc_tools_qt/ToolsProtMsgInterface.h"
#include "cc_tools_qt/property/field.h"

#include "comms/Field.h"
#include "comms/field/ArrayList.h"

#include <QtCore/QString>

#include <cassert>
#include <tuple>


namespace cc_tools_qt
{

namespace details
{

template <typename TFieldBase>
class ToolsRawDataMessageField : public comms::field::ArrayList<TFieldBase, std::uint8_t>
{
public:    
    static const char* name()
    {
        return "Data";
    }
};

template <typename TMsgBase, typename...>
class ToolsRawDataMessageImpl : public
    comms::MessageBase<
        TMsgBase,
        comms::option::NoIdImpl,
        comms::option::FieldsImpl<std::tuple<ToolsRawDataMessageField<typename TMsgBase::Field>>>,
        comms::option::MsgType<ToolsRawDataMessageImpl<TMsgBase>>
    >
{
};

}  // namespace details

/// @brief Raw data message.
template<typename TBase>
class ToolsRawDataMessage : public 
    cc_tools_qt::ToolsMessageBase<
        TBase,
        details::ToolsRawDataMessageImpl,
        ToolsRawDataMessage<TBase>
    >
{
public:
    virtual ~ToolsRawDataMessage() noexcept = default;

protected:
    virtual const char*
    nameImpl() const override
    {
        static const char* Str = "Generic Raw Data Message";
        return Str;
    }

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
            property::field::ArrayList().asMap());
        return props;
    }
};

}  // namespace cc_tools_qt



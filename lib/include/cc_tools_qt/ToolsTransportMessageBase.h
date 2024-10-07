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
#include "cc_tools_qt/ToolsMessageInterface.h"

#include <QtCore/QString>

#include <cassert>


namespace cc_tools_qt
{

/// @brief Base class for @b TransportMessage definition in @b protocol
///     plugin.
/// @tparam TProtMsg TODO
/// @tparam TActualMsg Type of the actual message class inheriting from this one
/// @tparam TBase Base class that this class is expected to inherit. Expected to be cc_tools_qt::Message or derivative.
/// @headerfile cc_tools_qt/ToolTransportMessageBase.h
template <typename TProtMsg, typename TActualMsg, typename TBase = cc_tools_qt::Message>
class ToolsTransportMessageBase : public
    cc_tools_qt::ToolsMessageBase<
        TProtMsg,
        TActualMsg,
        TBase
    >
{
public:
    /// @brief Destructor
    virtual ~ToolsTransportMessageBase() noexcept = default;

protected:
    /// @brief Overriding virtual cc_tools_qt::Message::nameImpl()
    virtual const char* nameImpl() const override
    {
        static const char* Str = "Generic Transport Message";
        return Str;
    }

    /// @brief Overriding virtual cc_tools_qt::Message::extraTransportFieldsPropertiesImpl()
    virtual const QVariantList&  extraTransportFieldsPropertiesImpl() const override
    {
        static const QVariantList List;
        return List;
    }

    virtual qlonglong numericIdImpl() const override
    {
        [[maybe_unused]] static constexpr bool Must_be_overriden = false;
        assert(Must_be_overriden); 
        return 0;
    }

    /// @brief Overriding virtual cc_tools_qt::Message::resetImpl()
    virtual void resetImpl() override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }

    /// @brief Overriding virtual cc_tools_qt::Message::assignImpl()
    virtual bool assignImpl([[maybe_unused]] const cc_tools_qt::Message& other) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return false;
    }
};

}  // namespace cc_tools_qt



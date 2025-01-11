//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsMessageBase.h"

#include "comms/MessageBase.h"
#include "comms/options.h"

#include <QtCore/QString>

#include <cassert>

namespace cc_tools_qt
{


/// @brief Base class for @b TransportMessage definition in @b protocol
///     plugin.
/// @tparam TBase Base class that this class is expected to inherit. Expected to be @ref cc_tools_qt::ToolsMessage or derivative.
/// @tparam TProtMsg Message class defined using @b comms::MessageBase class.
/// @tparam TActualMsg Type of the actual message class inheriting from this one
/// @headerfile cc_tools_qt/ToolsTransportMessageBase.h
template <typename TBase, template<typename...> class TProtMsg, typename TActualMsg>
class ToolsTransportMessageBase : public
    cc_tools_qt::ToolsMessageBase<
        TBase,
        TProtMsg,
        TActualMsg
    >
{
    using Base = 
        cc_tools_qt::ToolsMessageBase<
            TBase,
            TProtMsg,
            TActualMsg
        >;    
public:
    using FieldsList = typename Base::FieldsList;

    /// @brief Destructor
    virtual ~ToolsTransportMessageBase() noexcept = default;

protected:
    /// @brief Overriding virtual cc_tools_qt::ToolsMessage::nameImpl()
    virtual const char* nameImpl() const override
    {
        static const char* Str = "Generic Transport Message";
        return Str;
    }

    virtual qlonglong numericIdImpl() const override
    {
        [[maybe_unused]] static constexpr bool Must_be_overriden = false;
        assert(Must_be_overriden); 
        return 0;
    }

    /// @brief Overriding virtual cc_tools_qt::ToolsMessage::resetImpl()
    virtual void resetImpl() override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
    }

    /// @brief Overriding virtual cc_tools_qt::ToolsMessage::assignImpl()
    virtual bool assignImpl([[maybe_unused]] const cc_tools_qt::ToolsMessage& other) override
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called); 
        return false;
    }

    virtual FieldsList transportFieldsImpl() override
    {
        return FieldsList();
    }
};

}  // namespace cc_tools_qt



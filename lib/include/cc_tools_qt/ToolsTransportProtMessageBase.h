//
// Copyright 2024 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsProtMsgInterface.h"

#include "comms/MessageBase.h"


namespace cc_tools_qt
{

template <template<typename...> class TMsgBase, typename TFields, typename TActualMsg>
class ToolsTransportProtMessageBase : public
    comms::MessageBase<
        ToolsProtMsgInterface<TMsgBase>,
        comms::option::NoIdImpl,
        comms::option::FieldsImpl<TFields>,
        comms::option::MsgType<TActualMsg>
    >
{
};

}  // namespace cc_tools_qt



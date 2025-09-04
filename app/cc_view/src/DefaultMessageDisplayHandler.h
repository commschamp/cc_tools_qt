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

#pragma once

#include <cassert>
#include <type_traits>

#include <QtWidgets/QVBoxLayout>

#include "comms/comms.h"

#include "cc_tools_qt/ToolsMessage.h"
#include "widget/DefaultMessageWidget.h"

namespace cc_tools_qt
{

class DefaultMessageDisplayHandler
{
public:
    using MsgWidgetPtr = std::unique_ptr<MessageWidget>;

    ~DefaultMessageDisplayHandler() noexcept;

    static MsgWidgetPtr getMsgWidget(ToolsMessage& msg);
};

}  // namespace cc_tools_qt


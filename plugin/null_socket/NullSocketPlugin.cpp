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

#include "NullSocketPlugin.h"

#include "NullSocket.h"

#include <memory>
#include <cassert>

namespace cc_tools_qt
{

namespace plugin
{

NullSocketPlugin::NullSocketPlugin() :
    Base(Type_Socket)
{
}

NullSocketPlugin::~NullSocketPlugin() noexcept = default;

ToolsSocketPtr NullSocketPlugin::createSocketImpl()
{
    return makeNullSocket();
}


}  // namespace plugin

}  // namespace cc_tools_qt



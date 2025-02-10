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

#include <memory>

#include "cc_tools_qt/ToolsPlugin.h"
#include "cc_tools_qt/ToolsSocket.h"

namespace cc_tools_qt
{

namespace plugin
{

class EchoSocketPlugin : public cc_tools_qt::ToolsPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "cc.EchoSocketPlugin" FILE "echo_socket.json")
    Q_INTERFACES(cc_tools_qt::ToolsPlugin)

    using Base = cc_tools_qt::ToolsPlugin;

public:
    EchoSocketPlugin();
    ~EchoSocketPlugin() noexcept;

protected:
    virtual ToolsSocketPtr createSocketImpl() override;    

private:
    cc_tools_qt::ToolsSocketPtr m_socket;
};

}  // namespace plugin

}  // namespace cc_tools_qt





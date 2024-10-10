//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/Plugin.h"

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

namespace demo
{

namespace cc_plugin
{

class DemoPlugin : public cc_tools_qt::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "cc.DemoProtocol" FILE "demo.json")
    Q_INTERFACES(cc_tools_qt::Plugin)

public:
    DemoPlugin();
    ~DemoPlugin() noexcept;
};

}  // namespace cc_plugin

}  // namespace demo


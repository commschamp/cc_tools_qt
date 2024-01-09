//
// Copyright 2024 - 2025 (C). Alex Robenko. All rights reserved.
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
#include <vector>

#include <QtWidgets/QWidget>

#include "cc_tools_qt/PluginMgr.h"

#include "PluginConfigWrapWidget.h"

namespace cc_tools_qt
{

class PluginConfigWrapsListWidget: public QWidget
{
    using Base = QWidget;

public:
    using PluginInfoPtr = PluginMgr::PluginInfoPtr;
    explicit PluginConfigWrapsListWidget(QWidget* parentObj = nullptr);
    ~PluginConfigWrapsListWidget();

    void addPluginConfig(PluginInfoPtr pluginInfo);
    void removePluginConfig(PluginInfoPtr pluginInfo);
    void removeAll();

private:
    using PluginConfigWrapWidgetPtr = std::unique_ptr<PluginConfigWrapWidget>;
    using WrapWidgetsList = std::vector<PluginConfigWrapWidgetPtr>;
    WrapWidgetsList m_widgets;
};

} // namespace cc_tools_qt

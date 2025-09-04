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

#include "PluginConfigWrapWidget.h"

#include "cc_tools_qt/ToolsPluginMgr.h"

#include <QtWidgets/QWidget>

#include <memory>
#include <vector>

namespace cc_tools_qt
{

class PluginConfigWrapsListWidget: public QWidget
{
    using Base = QWidget;

public:
    using PluginInfoPtr = ToolsPluginMgr::PluginInfoPtr;
    explicit PluginConfigWrapsListWidget(QWidget* parentObj = nullptr);
    ~PluginConfigWrapsListWidget();

    void addPluginConfig(PluginInfoPtr pluginInfo);
    void removePluginConfig(PluginInfoPtr pluginInfo);
    void removeAll();
    void moveTop(PluginInfoPtr pluginInfo);
    void moveBottom(PluginInfoPtr pluginInfo);
    void moveUp(PluginInfoPtr pluginInfo);
    void moveDown(PluginInfoPtr pluginInfo);

private:
    int findWidgetIdx(PluginInfoPtr pluginInfo) const;
    int getTopFilterIdx() const;
    int getBottomFilterIdx() const;
    void relocateWidget(int from, int to);
    using PluginConfigWrapWidgetPtr = std::unique_ptr<PluginConfigWrapWidget>;
    using WrapWidgetsList = std::vector<PluginConfigWrapWidgetPtr>;
    WrapWidgetsList m_widgets;
    std::vector<ToolsPlugin*> m_loadedPlugins;
};

} // namespace cc_tools_qt

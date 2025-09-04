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

#include "ui_PluginConfigWrapWidget.h"

#include "cc_tools_qt/ToolsPluginMgr.h"

#include <QtWidgets/QWidget>

namespace cc_tools_qt
{

class PluginConfigWrapWidget: public QWidget
{
    using Base = QWidget;

public:
    using Type = ToolsPluginMgr::PluginInfo::Type;
    using PluginInfoPtr = ToolsPluginMgr::PluginInfoPtr;
    PluginConfigWrapWidget(PluginInfoPtr pluginInfo, QWidget* configWidget, QWidget* parentObj = nullptr);
    ~PluginConfigWrapWidget();

    const QString& getIid() const
    {
        return m_pluginInfo->getIid();
    }

    Type getType() const
    {
        return m_pluginInfo->getType();
    }    

private:
    Ui::PluginConfigWrapWidget m_ui;
    PluginInfoPtr m_pluginInfo;
};

} // namespace cc_tools_qt

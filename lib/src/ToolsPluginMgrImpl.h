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

#include <memory>
#include <list>
#include <array>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QPluginLoader>

#include "cc_tools_qt/ToolsPlugin.h"
#include "cc_tools_qt/ToolsPluginMgr.h"
#include "cc_tools_qt/ConfigMgr.h"


namespace cc_tools_qt
{

class ToolsPluginMgrImpl
{
public:

    using PluginLoaderPtr = ToolsPluginMgr::PluginLoaderPtr;
    using PluginInfo = ToolsPluginMgr::PluginInfo;
    using PluginInfoPtr = ToolsPluginMgr::PluginInfoPtr;
    using ListOfPluginInfos = ToolsPluginMgr::ListOfPluginInfos;

    ToolsPluginMgrImpl();
    ~ToolsPluginMgrImpl() noexcept;

    void setPluginsDir(const QString& pluginDir);
    const ListOfPluginInfos& getAvailablePlugins();
    const ListOfPluginInfos& getAppliedPlugins() const;
    void setAppliedPlugins(const ListOfPluginInfos& plugins);
    ListOfPluginInfos loadPluginsFromConfig(const QVariantMap& config);
    ListOfPluginInfos loadPluginsFromConfigFile(const QString& filename);
    bool savePluginsToConfigFile(const ListOfPluginInfos& infos, const QString& filename);
    ToolsPlugin* loadPlugin(const PluginInfo& info);
    bool hasAppliedPlugins() const;
    bool needsReload(const ListOfPluginInfos& infos) const;
    bool isProtocolChanging(const ListOfPluginInfos& infos) const;
    void unloadApplied();
    bool unloadAppliedPlugin(const PluginInfo& info);
    static QVariantMap getConfigForPlugins(const ListOfPluginInfos& infos);
    const QString& getLastFile() const;
    static const QString& getFilesFilter();

private:
    using PluginLoadersList = std::list<PluginLoaderPtr>;

    PluginInfoPtr readPluginInfo(const QString& filename);

    QString m_pluginDir;
    ListOfPluginInfos m_plugins;
    ListOfPluginInfos m_appliedPlugins;
    ConfigMgr m_configMgr;
};

}  // namespace cc_tools_qt



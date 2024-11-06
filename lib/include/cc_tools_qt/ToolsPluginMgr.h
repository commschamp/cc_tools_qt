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

#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QPluginLoader>

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsPlugin.h"

namespace cc_tools_qt
{

class ToolsPluginMgrImpl;
class CC_TOOLS_API ToolsPluginMgr
{
public:

    using PluginLoaderPtr = std::shared_ptr<QPluginLoader>;

    class PluginInfo
    {
        friend class ToolsPluginMgr;
        friend class ToolsPluginMgrImpl;

    public:
        enum class Type
        {
            Invalid,
            Socket,
            Filter,
            Protocol,
            NumOfValues
        };

        const QString& getIid() const
        {
            return m_iid;
        }

        const QString& getName() const
        {
            return m_name;
        }

        const QString& getDescription() const
        {
            return m_desc;
        }

        Type getType() const
        {
            return m_type;
        }

    private:
        PluginInfo() = default;

        PluginLoaderPtr m_loader;
        QString m_iid;
        QString m_name;
        QString m_desc;
        Type m_type = Type::Invalid;
        bool m_applied = false;
    };

    using PluginInfoPtr = std::shared_ptr<PluginInfo>;
    using ListOfPluginInfos = std::list<PluginInfoPtr>;

    ToolsPluginMgr();
    ~ToolsPluginMgr() noexcept;

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
    std::unique_ptr<ToolsPluginMgrImpl> m_impl;
};

}  // namespace cc_tools_qt

Q_DECLARE_METATYPE(cc_tools_qt::ToolsPluginMgr::PluginInfoPtr);


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

#include "cc_tools_qt/ToolsPluginMgr.h"

#include "ToolsPluginMgrImpl.h"

namespace cc_tools_qt
{

namespace
{

struct MetaTypesRegistrator
{
    MetaTypesRegistrator()
    {
        qRegisterMetaType<ToolsPluginMgr::PluginInfoPtr>();
    }
};

void registerMetaTypesIfNeeded()
{
    [[maybe_unused]] static const MetaTypesRegistrator Registrator;
}

} // namespace

QString ToolsPluginMgr::PluginInfo::getNameWithVersion() const
{
    if (getVersion().isEmpty()) {
        return getName();
    }

    return getName() + " (" + getVersion() + ")";
}

ToolsPluginMgr::ToolsPluginMgr()
  : m_impl(new ToolsPluginMgrImpl())
{
    registerMetaTypesIfNeeded();
}

ToolsPluginMgr::~ToolsPluginMgr() noexcept = default;

void ToolsPluginMgr::setPluginsDir(const QString& pluginDir)
{
    m_impl->setPluginsDir(pluginDir);
}

const ToolsPluginMgr::ListOfPluginInfos& ToolsPluginMgr::getAvailablePlugins()
{
    return m_impl->getAvailablePlugins();
}

const ToolsPluginMgr::ListOfPluginInfos& ToolsPluginMgr::getAppliedPlugins() const
{
    return m_impl->getAppliedPlugins();
}

void ToolsPluginMgr::setAppliedPlugins(const ListOfPluginInfos& plugins)
{
    m_impl->setAppliedPlugins(plugins);
}

ToolsPluginMgr::ListOfPluginInfos ToolsPluginMgr::loadPluginsFromConfig(
    const QVariantMap& config)
{
    return m_impl->loadPluginsFromConfig(config);
}

ToolsPluginMgr::ListOfPluginInfos ToolsPluginMgr::loadPluginsFromConfigFile(
    const QString& filename)
{
    return m_impl->loadPluginsFromConfigFile(filename);
}

bool ToolsPluginMgr::savePluginsToConfigFile(
    const ListOfPluginInfos& infos,
    const QString& filename)
{
    return m_impl->savePluginsToConfigFile(infos, filename);
}

ToolsPlugin* ToolsPluginMgr::loadPlugin(const PluginInfo& info)
{
    return m_impl->loadPlugin(info);
}

bool ToolsPluginMgr::hasAppliedPlugins() const
{
    return m_impl->hasAppliedPlugins();
}

bool ToolsPluginMgr::needsReload(const ListOfPluginInfos& infos) const
{
    return m_impl->needsReload(infos);
}

bool ToolsPluginMgr::isProtocolChanging(const ListOfPluginInfos& infos) const
{
    return m_impl->isProtocolChanging(infos);
}

void ToolsPluginMgr::unloadApplied()
{
    m_impl->unloadApplied();
}

bool ToolsPluginMgr::unloadAppliedPlugin(const PluginInfo& info)
{
    return m_impl->unloadAppliedPlugin(info);
}

QVariantMap ToolsPluginMgr::getConfigForPlugins(
    const ListOfPluginInfos& infos)
{
    return ToolsPluginMgrImpl::getConfigForPlugins(infos);
}

const QString& ToolsPluginMgr::getLastFile() const
{
    return m_impl->getLastFile();
}

const QString& ToolsPluginMgr::getFilesFilter()
{
    return ToolsPluginMgrImpl::getFilesFilter();
}

}  // namespace cc_tools_qt


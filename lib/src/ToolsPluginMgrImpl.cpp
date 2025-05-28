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

#include "ToolsPluginMgrImpl.h"

#include <cassert>
#include <algorithm>
#include <type_traits>
#include <iostream>

#include <QtCore/QString>
#include <QtCore/QVariantList>
#include <QtCore/QDir>
#include <QtCore/QJsonArray>
#include <QtCore/QVariantList>

#include "cc_tools_qt/ToolsPlugin.h"

namespace cc_tools_qt
{

namespace
{

const QString PluginsKey("cc_plugins_list");
const QString IidMetaKey("IID");
const QString MetaDataMetaKey("MetaData");
const QString NameMetaKey("name");
const QString DescMetaKey("desc");
const QString TypeMetaKey("type");
const QString VersionMetaKey("version");

struct PluginLoaderDeleter
{
    void operator()(QPluginLoader* loader)
    {
        assert(loader != nullptr);
        if (loader->isLoaded()) {
            loader->unload();
        }
        delete loader;
    }
};

ToolsPlugin* getPlugin(QPluginLoader& loader)
{
    auto* inst = loader.instance();
    auto* plugin = qobject_cast<ToolsPlugin*>(loader.instance());

    do {
        if (plugin != nullptr) {
            break;
        }

        if (inst == nullptr) {
            std::cerr << "ERROR: The selected library \"" <<
                loader.fileName().toStdString() << "\" is not a Qt plugin: " <<
                loader.errorString().toStdString() << std::endl;
            break;
        }

        std::cerr << "ERROR: The selected library \"" <<
            loader.fileName().toStdString() << "\" is not a cc_tools_qt plugin!" << std::endl;
    } while (false);
    return plugin;
}

ToolsPluginMgrImpl::PluginInfo::Type parseType(const QString& val)
{
    static const QString Values[] = {
        QString(),
        "socket",
        "filter",
        "protocol"
    };

    static_assert(
        std::extent<decltype(Values)>::value == static_cast<std::size_t>(ToolsPluginMgrImpl::PluginInfo::Type::NumOfValues),
        "The Values array must be adjusted.");

    auto iter = std::find(std::begin(Values), std::end(Values), val);
    if (iter == std::end(Values)) {
        return ToolsPluginMgrImpl::PluginInfo::Type::Invalid;
    }

    return static_cast<ToolsPluginMgrImpl::PluginInfo::Type>(std::distance(std::begin(Values), iter));
}

}  // namespace

ToolsPluginMgrImpl::ToolsPluginMgrImpl() = default;

ToolsPluginMgrImpl::~ToolsPluginMgrImpl() noexcept
{
    for (auto& pluginInfoPtr : m_plugins) {
        assert(pluginInfoPtr);
        assert(pluginInfoPtr->m_loader);
        if (pluginInfoPtr->m_loader->isLoaded()) {
            pluginInfoPtr->m_loader->unload();
        }
    }
}

void ToolsPluginMgrImpl::setPluginsDir(const QString& pluginDir)
{
    m_pluginDir = pluginDir;
}

const ToolsPluginMgrImpl::ListOfPluginInfos& ToolsPluginMgrImpl::getAvailablePlugins()
{
    if (!m_plugins.empty()) {
        return m_plugins;
    }

    do {
        QDir pluginDir(m_pluginDir);
        auto files =
            pluginDir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

        for (auto& f : files) {
            auto infoPtr = readPluginInfo(f);
            if (!infoPtr) {
                continue;
            }

            if (infoPtr->getType() == PluginInfo::Type::Invalid) {
                std::cerr << "WARNING: plugin " << f.toStdString() << " doesn't specify its type, use either "
                    "\"socket\", or \"filter\", or \"protocol\"."<< std::endl;
                continue;
            }

            m_plugins.push_back(std::move(infoPtr));
        }
    } while (false);

    m_plugins.sort(
        [](auto& info1, auto& info2)
        {
            return info1->getName().compare(info2->getName(), Qt::CaseInsensitive) < 0;
        });

    return m_plugins;
}

const ToolsPluginMgrImpl::ListOfPluginInfos& ToolsPluginMgrImpl::getAppliedPlugins() const
{
    return m_appliedPlugins;
}

void ToolsPluginMgrImpl::setAppliedPlugins(const ListOfPluginInfos& plugins)
{
    m_appliedPlugins = plugins;
}

ToolsPluginMgrImpl::ListOfPluginInfos ToolsPluginMgrImpl::loadPluginsFromConfig(
    const QVariantMap& config)
{
    ListOfPluginInfos pluginInfos;
    do {
        auto listVar = config.value(PluginsKey);
        if ((!listVar.isValid()) || (!listVar.canConvert<QVariantList>())) {
            break;
        }

        auto varList = listVar.value<QVariantList>();
        auto& availPlugins = getAvailablePlugins();
        for (auto& iidVar : varList) {
            if ((!iidVar.isValid()) || (!iidVar.canConvert<QString>())) {
                continue;
            }

            auto iid = iidVar.toString();
            auto iter =
                std::find_if(
                    availPlugins.begin(), availPlugins.end(),
                    [&iid](const PluginInfoPtr& i) -> bool
                    {
                        return i->m_iid == iid;
                    });

            if (iter == m_plugins.end()) {
                continue;
            }

            auto pluginInfoPtr = *iter;
            assert(pluginInfoPtr);
            assert(pluginInfoPtr->m_loader);
            auto* pluginPtr = getPlugin(*pluginInfoPtr->m_loader);
            assert(pluginPtr != nullptr);
            pluginPtr->reconfigure(config);

            pluginInfos.push_back(*iter);
        }

    } while (false);

    return pluginInfos;
}

ToolsPluginMgrImpl::ListOfPluginInfos ToolsPluginMgrImpl::loadPluginsFromConfigFile(
    const QString& filename)
{
    auto config = m_configMgr.loadConfig(filename);
    if (config.isEmpty()) {
        return ListOfPluginInfos();
    }

    return loadPluginsFromConfig(config);
}

bool ToolsPluginMgrImpl::savePluginsToConfigFile(
    const ListOfPluginInfos& infos,
    const QString& filename)
{
    auto config = getConfigForPlugins(infos);
    return m_configMgr.saveConfig(filename, config);
}

ToolsPlugin* ToolsPluginMgrImpl::loadPlugin(const PluginInfo& info)
{
    return getPlugin(*info.m_loader);
}

bool ToolsPluginMgrImpl::hasAppliedPlugins() const
{
    return !m_appliedPlugins.empty();
}

bool ToolsPluginMgrImpl::needsReload(const ListOfPluginInfos& infos) const
{
    assert(!infos.empty());
    return (!m_appliedPlugins.empty()) &&
           (m_appliedPlugins != infos);
}

bool ToolsPluginMgrImpl::isProtocolChanging(const ListOfPluginInfos& infos) const
{
    auto findProtocolFunc =
        [](const ListOfPluginInfos& l) -> PluginInfoPtr
        {
            auto iter =
                std::find_if(l.rbegin(), l.rend(),
                    [](const PluginInfoPtr& ptr) -> bool
                    {
                        return ptr->getType() == ToolsPluginMgr::PluginInfo::Type::Protocol;
                    });

            if (iter == l.rend()) {
                return PluginInfoPtr();
            }

            return *iter;
        };

    auto newProtocol = findProtocolFunc(infos);
    auto appliedProtocol = findProtocolFunc(m_appliedPlugins);
    return newProtocol != appliedProtocol;
}

void ToolsPluginMgrImpl::unloadApplied()
{
    for (auto& pluginInfo : m_appliedPlugins) {
        assert(pluginInfo);
        assert(pluginInfo->m_loader);
        assert (pluginInfo->m_loader->isLoaded());
        pluginInfo->m_loader->unload();
    }
    m_appliedPlugins.clear();
}

bool ToolsPluginMgrImpl::unloadAppliedPlugin(const PluginInfo& info)
{
    auto iter =
        std::find_if(
            m_appliedPlugins.begin(), m_appliedPlugins.end(),
            [&info](const PluginInfoPtr& ptr) -> bool
            {
                return ptr.get() == &info;
            });

    if (iter == m_appliedPlugins.end()) {
        return false;
    }

    auto pluginInfoPtr = *iter;
    m_appliedPlugins.erase(iter);

    assert(pluginInfoPtr);
    assert(pluginInfoPtr->m_loader);
    assert (pluginInfoPtr->m_loader->isLoaded());
    pluginInfoPtr->m_loader->unload();
    return true;
}

QVariantMap ToolsPluginMgrImpl::getConfigForPlugins(
    const ListOfPluginInfos& infos)
{
    QVariantMap config;
    QVariantList pluginsList;
    for (auto& pluginInfoPtr : infos) {
        assert(pluginInfoPtr);
        assert(!pluginInfoPtr->m_iid.isEmpty());
        pluginsList.append(QVariant::fromValue(pluginInfoPtr->m_iid));

        assert(pluginInfoPtr->m_loader);
        auto* pluginPtr = getPlugin(*pluginInfoPtr->m_loader);
        assert(pluginPtr != nullptr);
        pluginPtr->getCurrentConfig(config);
    }

    config.insert(PluginsKey, QVariant::fromValue(pluginsList));
    return config;
}

const QString& ToolsPluginMgrImpl::getLastFile() const
{
    return m_configMgr.getLastFile();
}

const QString& ToolsPluginMgrImpl::getFilesFilter()
{
    return ToolsConfigMgr::getFilesFilter();
}

ToolsPluginMgrImpl::PluginInfoPtr ToolsPluginMgrImpl::readPluginInfo(const QString& filename)
{
    PluginInfoPtr ptr;

    do {
        PluginLoaderPtr loader(new QPluginLoader(filename));
        assert(!loader->isLoaded());
        auto metaData = loader->metaData();
        assert(!loader->isLoaded());

        if (metaData.isEmpty()) {
            break;
        }

        auto iidJsonVal = metaData.value(IidMetaKey);
        if (!iidJsonVal.isString()) {
            break;
        }

        ptr.reset(new PluginInfo());
        ptr->m_iid = iidJsonVal.toString();
        ptr->m_loader = std::move(loader);

        auto extraMeta = metaData.value(MetaDataMetaKey);
        if (!extraMeta.isObject()) {
            ptr->m_name = ptr->m_iid;
            break;
        }

        auto extraMetaObj = extraMeta.toObject();
        auto nameJsonVal = extraMetaObj.value(NameMetaKey);
        if (!nameJsonVal.isString()) {
            ptr->m_name = ptr->m_iid;
        }

        auto nameStr = nameJsonVal.toString();
        if (nameStr.isEmpty()) {
            ptr->m_name = ptr->m_iid;
        }
        else {
            ptr->m_name = nameStr;
        }

        auto descJsonVal = extraMetaObj.value(DescMetaKey);
        if (descJsonVal.isString()) {
            ptr->m_desc = descJsonVal.toString();
        }
        else if (descJsonVal.isArray()) {
            auto descVarList = descJsonVal.toArray().toVariantList();
            for (auto& descPartVar : descVarList) {
                if ((descPartVar.isValid()) &&
                    (descPartVar.canConvert<QString>())) {
                    ptr->m_desc.append(descPartVar.toString());
                }
            }
        }

        auto typeJsonVal = extraMetaObj.value(TypeMetaKey);
        ptr->m_type = parseType(typeJsonVal.toString().toLower());

        ptr->m_version = extraMetaObj.value(VersionMetaKey).toString();

    } while (false);
    return ptr;
}

}  // namespace cc_tools_qt



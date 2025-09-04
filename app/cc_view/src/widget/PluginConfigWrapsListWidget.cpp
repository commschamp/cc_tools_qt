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

#include "PluginConfigWrapsListWidget.h"

#include "PluginMgrG.h"

#include <algorithm>
#include <cassert>
#include <iterator>

namespace cc_tools_qt
{

PluginConfigWrapsListWidget::PluginConfigWrapsListWidget(QWidget* parentObj) :
    Base(parentObj)
{
    auto* layout = new QVBoxLayout;
    layout->addStretch(1);
    setLayout(layout);
}

PluginConfigWrapsListWidget::~PluginConfigWrapsListWidget() = default;

void PluginConfigWrapsListWidget::addPluginConfig(PluginInfoPtr pluginInfo)
{
    auto* plugin = PluginMgrG::instanceRef().loadPlugin(*pluginInfo);
    if (plugin == nullptr) {
        return;
    }

    assert(std::find(m_loadedPlugins.begin(), m_loadedPlugins.end(), plugin) == m_loadedPlugins.end());
    disconnect(plugin, &ToolsPlugin::sigInterPluginConfigReport, this, nullptr);
    connect(
        plugin, &ToolsPlugin::sigInterPluginConfigReport,
        this, 
        [this, plugin](const QVariantMap& props)
        {
            for (auto* p : m_loadedPlugins) {
                if (p == plugin) {
                    continue;
                }

                p->applyInterPluginConfig(props);
            }
        });

    m_loadedPlugins.push_back(plugin);

    auto* configWidget = plugin->createConfigurationWidget();
    if (configWidget == nullptr) {
        return;
    }

    auto type = pluginInfo->getType();
    auto iter = std::upper_bound(
        m_widgets.begin(), m_widgets.end(), type,
        [](auto t, auto& w)
        {
            return t < w->getType();
        });

    auto idx = static_cast<int>(std::distance(m_widgets.begin(), iter));
    
    auto* l = qobject_cast<QVBoxLayout*>(layout());

    auto wrap = std::make_unique<PluginConfigWrapWidget>(pluginInfo, configWidget, this);
    l->insertWidget(idx, wrap.get());
    m_widgets.insert(iter, std::move(wrap));
}

void PluginConfigWrapsListWidget::removePluginConfig(PluginInfoPtr pluginInfo)
{
    auto* plugin = PluginMgrG::instanceRef().loadPlugin(*pluginInfo);
    auto pluginIter = std::find(m_loadedPlugins.begin(), m_loadedPlugins.end(), plugin);
    if (pluginIter != m_loadedPlugins.end()) {
        m_loadedPlugins.erase(pluginIter);
    }

    auto idx = findWidgetIdx(pluginInfo);
    if (idx < 0) {
        return;
    }

    m_widgets.erase(m_widgets.begin() + idx); // Will remove widget as well
}

void PluginConfigWrapsListWidget::removeAll()
{
    m_widgets.clear();
    m_loadedPlugins.clear();
}

void PluginConfigWrapsListWidget::moveTop(PluginInfoPtr pluginInfo)
{
    if (pluginInfo->getType() != ToolsPluginMgr::PluginInfo::Type::Filter) {
        return;
    }

    auto idx = findWidgetIdx(pluginInfo);
    auto topIdx = getTopFilterIdx();

    if ((idx < 0) || (idx <= topIdx)) {
        return;
    }

    relocateWidget(idx, topIdx);    
}

void PluginConfigWrapsListWidget::moveBottom(PluginInfoPtr pluginInfo)
{
    if (pluginInfo->getType() != ToolsPluginMgr::PluginInfo::Type::Filter) {
        return;
    }

    auto idx = findWidgetIdx(pluginInfo);
    auto bottomIdx = getBottomFilterIdx();
    if ((idx < 0) || (bottomIdx <= idx)) {
        return;
    }

    relocateWidget(idx, bottomIdx);
}

void PluginConfigWrapsListWidget::moveUp(PluginInfoPtr pluginInfo)
{
    if (pluginInfo->getType() != ToolsPluginMgr::PluginInfo::Type::Filter) {
        return;
    }

    auto idx = findWidgetIdx(pluginInfo);
    auto topIdx = getTopFilterIdx();
    if ((idx <= 0) || (idx <= topIdx)) {
        return;
    }

    relocateWidget(idx, idx - 1);
}

void PluginConfigWrapsListWidget::moveDown(PluginInfoPtr pluginInfo)
{
    if (pluginInfo->getType() != ToolsPluginMgr::PluginInfo::Type::Filter) {
        return;
    }

    auto idx = findWidgetIdx(pluginInfo);
    auto bottomIdx = getBottomFilterIdx();
    if ((idx < 0) || (bottomIdx <= idx)) {
        return;
    }

    relocateWidget(idx, idx + 1);
}

int PluginConfigWrapsListWidget::findWidgetIdx(PluginInfoPtr pluginInfo) const
{
    auto iid = pluginInfo->getIid();
    auto iter = 
        std::find_if(
            m_widgets.begin(), m_widgets.end(), 
            [&iid](auto& w)
            {
                return w->getIid() == iid;
            });

    if (iter == m_widgets.end()) {
        return -1;
    }

    return static_cast<int>(std::distance(m_widgets.begin(), iter));
}

int PluginConfigWrapsListWidget::getTopFilterIdx() const
{
    auto iter = std::lower_bound(
        m_widgets.begin(), m_widgets.end(), ToolsPluginMgr::PluginInfo::Type::Filter,
        [](auto& w, auto t)
        {
            return w->getType() < t;
        });    

    return static_cast<int>(std::distance(m_widgets.begin(), iter)); 
}

int PluginConfigWrapsListWidget::getBottomFilterIdx() const
{
    auto iter = std::upper_bound(
        m_widgets.begin(), m_widgets.end(), ToolsPluginMgr::PluginInfo::Type::Filter,
        [](auto t, auto& w)
        {
            return t < w->getType();
        });    

    return static_cast<int>(std::distance(m_widgets.begin(), iter)); 
}

void PluginConfigWrapsListWidget::relocateWidget(int from, int to)
{
    assert(from != to);

    auto* l = qobject_cast<QVBoxLayout*>(layout());

    auto* item = l->takeAt(from);
    auto iter = m_widgets.begin() + from;

    auto wrap = std::move(*iter);
    m_widgets.erase(iter);

    to = std::min(to, static_cast<int>(m_widgets.size()));

    l->insertItem(to, item);
    m_widgets.insert(m_widgets.begin() + to, std::move(wrap));
}

} // namespace cc_tools_qt

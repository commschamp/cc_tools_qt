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

#include <algorithm>
#include <cassert>
#include <iterator>

#include "PluginMgrG.h"

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

    auto* configWidget = plugin->createConfiguarionWidget();
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
    auto iid = pluginInfo->getIid();
    auto iter = 
        std::find_if(
            m_widgets.begin(), m_widgets.end(), 
            [&iid](auto& w)
            {
                return w->getIid() == iid;
            });

    if (iter == m_widgets.end()) {
        return;
    }

    m_widgets.erase(iter); // Will remove widget as well
}

void PluginConfigWrapsListWidget::removeAll()
{
    m_widgets.clear();
}

} // namespace cc_tools_qt
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

#include "PluginConfigWrapWidget.h"

#include <cassert>

namespace cc_tools_qt
{

PluginConfigWrapWidget::PluginConfigWrapWidget(PluginInfoPtr pluginInfo, QWidget* configWidget, QWidget* parentObj) :
    Base(parentObj)
{
    assert(configWidget != nullptr);
    m_ui.setupUi(this);
    auto* layout = qobject_cast<QVBoxLayout*>(m_ui.m_wrapGroupBox->layout());
    layout->addWidget(configWidget);

    m_ui.m_wrapGroupBox->setTitle(pluginInfo->getName());
    m_pluginInfo = pluginInfo;
}

PluginConfigWrapWidget::~PluginConfigWrapWidget() = default;

} // namespace cc_tools_qt
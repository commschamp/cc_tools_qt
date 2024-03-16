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

#include "cc_tools_qt/Plugin.h"

namespace cc_tools_qt
{

namespace
{

template <typename TFunc>
auto invokeCreationFunc(TFunc&& func) -> decltype(func())
{
    typedef decltype(func()) RetType;

    if (!func) {
        return RetType();
    }

    return func();
}

}  // namespace

Plugin::Plugin() = default;
Plugin::~Plugin() noexcept = default;

void Plugin::getCurrentConfig(QVariantMap& config)
{
    getCurrentConfigImpl(config);
}

QVariantMap Plugin::getCurrentConfig()
{
    QVariantMap config;
    getCurrentConfig(config);
    return config;
}

void Plugin::reconfigure(const QVariantMap& config)
{
    reconfigureImpl(config);
}

SocketPtr Plugin::createSocket() const
{
    return invokeCreationFunc(m_props.getSocketCreateFunc());
}

Plugin::ListOfFilters Plugin::createFilters() const
{
    return invokeCreationFunc(m_props.getFiltersCreateFunc());
}

ProtocolPtr Plugin::createProtocol() const
{
    return invokeCreationFunc(m_props.getProtocolCreateFunc());
}

Plugin::ListOfGuiActions Plugin::createGuiActions() const
{
    return invokeCreationFunc(m_props.getGuiActionsCreateFunc());
}

QWidget* Plugin::createConfiguarionWidget() const
{
    auto func = m_props.getConfigWidgetCreateFunc();
    if (!func) {
        return nullptr;
    }

    return func();
}

QVariant Plugin::getCustomProperty(const QString& name)
{
    return m_props.getCustomProperty(name);
}

void Plugin::applyInterPluginConfig(const QVariantMap& props)
{
    applyInterPluginConfigImpl(props);
}

void Plugin::setInterPluginConfigReportCallback(InterPluginConfigReportCallback&& func)
{
    m_interPluginConfigReportCallback = std::move(func);
}    

void Plugin::getCurrentConfigImpl([[maybe_unused]] QVariantMap& config)
{
}

void Plugin::reconfigureImpl([[maybe_unused]] const QVariantMap& config)
{
}

void Plugin::applyInterPluginConfigImpl([[maybe_unused]] const QVariantMap& props)
{
}

PluginProperties& Plugin::pluginProperties()
{
    return m_props;
}

void Plugin::reportInterPluginConfig(const QVariantMap& props)
{
    if (m_interPluginConfigReportCallback) {
        m_interPluginConfigReportCallback(props);
    }
}

}  // namespace cc_tools_qt



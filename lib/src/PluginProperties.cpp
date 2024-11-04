//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/PluginProperties.h"

#include <cassert>

namespace cc_tools_qt
{

namespace
{

const QString PropPrefix("cc.PluginProp_");
const QString SocketCreateFuncPropName = PropPrefix + "socket";
const QString ProtocolCreateFuncPropName = PropPrefix + "protocol";
const QString FiltersCreateFuncPropName = PropPrefix + "filters";
const QString GuiActionsCreateFuncPropName = PropPrefix + "gui_actions";
const QString ConfigWidgetCreateFuncPropName = PropPrefix + "config_widget";

template <typename TFunc>
TFunc getFuncProperty(const QVariantMap& props, const QString& name)
{
    auto var = props.value(name);
    if ((!var.isValid()) ||
        (!var.canConvert<TFunc>())) {
        return TFunc();
    }

    return var.value<TFunc>();
}

}  // namespace

PluginProperties::PluginProperties() = default;
PluginProperties::~PluginProperties() noexcept = default;


PluginProperties& PluginProperties::setGuiActionsCreateFunc(
    GuiActionsCreateFunc&& func)
{
    m_props.insert(GuiActionsCreateFuncPropName, QVariant::fromValue(std::move(func)));
    return *this;
}

PluginProperties& PluginProperties::setConfigWidgetCreateFunc(
    ConfigWidgetCreateFunc&& func)
{
    m_props.insert(ConfigWidgetCreateFuncPropName, QVariant::fromValue(std::move(func)));
    return *this;
}

PluginProperties& PluginProperties::setCustomProperty(
    const QString& name,
    QVariant&& val)
{
    do {
        if (name.startsWith(PropPrefix)) {
            [[maybe_unused]] static constexpr bool Wrong_property_name = false;
            assert(Wrong_property_name); // Custom property cannot start with cc.PluginProp_           
            break;
        }

        m_props.insert(name, std::move(val));
    } while (false);
    return *this;
}

PluginProperties::GuiActionsCreateFunc PluginProperties::getGuiActionsCreateFunc() const
{
    return getFuncProperty<GuiActionsCreateFunc>(m_props, GuiActionsCreateFuncPropName);
}

PluginProperties::ConfigWidgetCreateFunc PluginProperties::getConfigWidgetCreateFunc() const
{
    return getFuncProperty<ConfigWidgetCreateFunc>(m_props, ConfigWidgetCreateFuncPropName);
}

QVariant PluginProperties::getCustomProperty(const QString& name) const
{
    if (name.startsWith(PropPrefix)) {
        [[maybe_unused]] static constexpr bool Wrong_property_name = false;
        assert(Wrong_property_name); // Custom property cannot start with cc.PluginProp_
        return QVariant();
    }

    return m_props.value(name);
}

}  // namespace cc_tools_qt



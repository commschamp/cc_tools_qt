//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsFilter.h"
#include "cc_tools_qt/ToolsProtocol.h"
#include "cc_tools_qt/ToolsSocket.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QtPlugin>
#include <QtCore/QVariantMap>

class QAction;
class QWidget;

namespace cc_tools_qt
{

/// @brief Interface class for plugin definition
/// @headerfile cc_tools_qt/ToolsPlugin.h
class CC_TOOLS_API ToolsPlugin : public QObject
{
public:
    enum Type : unsigned
    {
        Type_Socket,
        Type_Filter,
        Type_Protocol,
        Type_NumOfValues
    };

    /// @brief List of GUI action buttons
    using ListOfGuiActions = QList<QAction*>;

    /// @brief Constructor
    explicit ToolsPlugin(Type type);

    /// @brief Destructor
    virtual ~ToolsPlugin() noexcept;

    Type type() const;

    /// @brief Get current configuration.
    /// @details Invokes getCurrentConfigImpl(), which can be overridden by
    ///     the derived class. It is responsible to update the provided
    ///     configuration with new entries relevant to the plugin.
    /// @param[in, out] config Configuration map
    void getCurrentConfig(QVariantMap& config);

    /// @brief Get currentConfiguration.
    /// @details Invokes getCurrentConfigImpl(), which can be overridden by
    ///     the derived class. It is responsible to update the provided
    ///     configuration with new entries relevant to the plugin.
    /// @return Configuration map.
    QVariantMap getCurrentConfig();

    /// @brief Request to perform reconfigure.
    /// @details Invokes reconfigureImpl(), which can be overridden by the
    ///     derived class. The latter must retrieve appropriate values from
    ///     the configuration map and apply requested updates.
    void reconfigure(const QVariantMap& config);

    /// @brief Create socket
    /// @details This function will be called if it is @b socket plugin. It
    ///     will invoke socket creation callback function assigned by the
    ///     derived class to pluginProperties().
    /// @return Allocated socket object
    ToolsSocketPtr createSocket();

    ToolsFilterPtr createFilter();

    /// @brief Create protocol
    /// @details This function will be called if it is @b protocol plugin. It
    ///     will invoke protocol creation callback function assigned by the
    ///     derived class to pluginProperties().
    /// @return Allocated protocol object
    ToolsProtocolPtr createProtocol();


    /// @brief Create a widget to perform plugin configuration in GUI application.
    /// @details Sometimes there is a need to provide a way to configure the
    ///     plugin. A GUI application will call this function to retrieve a widget
    ///     which will control the configuration. The returned widged is
    ///     expected to be dynamically allocated and will be owned by the
    ///     GUI application, i.e. the plugin doesn't need to perform any
    ///     delete operation on it.
    /// @return Dynamically allocated widget object
    QWidget* createConfigurationWidget();    

    /// @brief Create GUI actions relevant to the plugin.
    /// @details This function will invoke the relevant callback function
    ///     assigned by the derived class to pluginProperties(). The callback
    ///     function is responsible to allocate and return a list of
    ///     @b QAction objects, which will appear to the main toolbar of the
    ///     GUI application. Note, that the main application will @b own the
    ///     allocated @b QAction objects and will delete them later. The plugin
    ///     doesn't need to do it explicityly.
    /// @return List of GUI QAction objects.
    ListOfGuiActions createGuiActions();

    /// @brief Apply inter-plugin configuration.
    /// @details Allows one plugin to influence the configuration of another.
    ///     This function will be called for all currently chosen plugins to override
    ///     current configuration. Invokes polymorphic @ref applyInterPluginConfigImpl().
    /// @param[in] props Properties map.
    void applyInterPluginConfig(const QVariantMap& props);  
        
    /// @brief Callback to report inter-plugin configuration updates
    using InterPluginConfigReportCallback = std::function <void (const QVariantMap&)>;

    /// @brief Set callback to report inter-plugin configuration.
    void setInterPluginConfigReportCallback(InterPluginConfigReportCallback&& func);

    /// @brief Set debug output level
    /// @param[in] level Debug level. If @b 0, debug output is disabled
    void setDebugOutputLevel(unsigned level = 0U);

protected:
    /// @brief Polymorphic call to retrieve current configuration
    /// @details Default implementation does nothing. The derived class
    ///     needs to override this function if the plugin requires or supports
    ///     configuration and update the provided map with relevant values.
    /// @param[in, out] config Configuration map
    virtual void getCurrentConfigImpl(QVariantMap& config);

    /// @brief Polymorphic call to perform plugin (re)configuration
    /// @details Default implementation does nothing. The derived class
    ///     needs to override byt function if the plugin requires or supports
    ///     (re)configuration.
    /// @param[in] config Configuration map
    virtual void reconfigureImpl(const QVariantMap& config);

    /// @brief Polymorphic inter-plugin configuration application.
    /// @details Invoked by the applyInterPluginConfig().
    /// @param[in] props Properties map.
    virtual void applyInterPluginConfigImpl(const QVariantMap& props);       

    virtual ToolsSocketPtr createSocketImpl();
    virtual ToolsFilterPtr createFilterImpl();
    virtual ToolsProtocolPtr createProtocolImpl();
    virtual QWidget* createConfigurationWidgetImpl();
    virtual ListOfGuiActions createGuiActionsImpl();

    /// @brief Report inter-plugin configuration.
    /// @details Sometimes configuration of one plugin may influence configuration of another.
    ///     Use this function to report inter-plugin configuration properties.
    ///     When invoked all other plugins are expected to get their respecitve 
    ///     @ref applyInterPluginConfig() functions invoked.
    /// @param[in] props Reported properties.
    void reportInterPluginConfig(const QVariantMap& props);       

private:
    Type m_type = Type_NumOfValues;
    InterPluginConfigReportCallback m_interPluginConfigReportCallback;
    unsigned m_debugOutputLevel = 0U;
};

}  // namespace cc_tools_qt

Q_DECLARE_INTERFACE(cc_tools_qt::ToolsPlugin, "cc.ToolsPlugin")



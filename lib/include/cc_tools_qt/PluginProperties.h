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


#pragma once

#include <functional>

#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QList>

#include "Socket.h"
#include "cc_tools_qt/ToolsProtocol.h"
#include "Filter.h"
#include "Api.h"

class QAction;

namespace cc_tools_qt
{

/// @brief Accumulation of various properties of @ref Plugin.
/// @headerfile cc_tools_qt/PluginProperties.h
class CC_API PluginProperties
{
public:
    /// @brief List of dynamically allocated @b QAction objects
    using ListOfGuiActions = QList<QAction*>;

    /// @brief Constructor
    PluginProperties();

    /// @brief Destructor
    ~PluginProperties() noexcept;

    /// @brief Type of callback to be used to allocate @b QAction to be
    ///     displayed in the main toolbar of GUI application.
    /// @details The allocated @b QAction objects will be owned by the
    ///     application. The plugin won't need to delete them explicitly.
    using GuiActionsCreateFunc = std::function<ListOfGuiActions ()>;

    /// @brief Assign callback for list of @b QAction allocation.
    /// @param[in] func Callback function
    /// @return `*this`
    PluginProperties& setGuiActionsCreateFunc(GuiActionsCreateFunc&& func);

    /// @brief Set custom property
    /// @param[in] name Name of the property
    /// @param[out] val Property value
    PluginProperties& setCustomProperty(const QString& name, QVariant&& val);

    /// @brief Retrieve GUI Actions creation callback.
    GuiActionsCreateFunc getGuiActionsCreateFunc() const;

    /// @brief Get custom property
    /// @param[in] name Property name
    /// @return Value of the property
    QVariant getCustomProperty(const QString& name) const;

private:
    QVariantMap m_props;
};

}  // namespace cc_tools_qt

Q_DECLARE_METATYPE(cc_tools_qt::PluginProperties::GuiActionsCreateFunc);



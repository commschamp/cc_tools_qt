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

#include <QtCore/QVariant>
#include <QtCore/QVariantMap>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace cc_tools_qt
{

/// @brief Information about incomming or outdoing data
/// @headerfile "cc_tools_qt/ToolsDataInfo.h"
struct CC_TOOLS_API ToolsDataInfo
{
    /// @brief Type of clock used for timestamping
    using TimestampClock = std::chrono::high_resolution_clock;

    /// @brief Type of timestamps
    using Timestamp = std::chrono::time_point<TimestampClock>;

    /// @brief Type of raw data sequence
    using DataSeq = std::vector<std::uint8_t>;

    /// @brief Type of extra properties storage
    using PropertiesMap = QVariantMap;

    ToolsDataInfo();
    Timestamp m_timestamp; ///< Timestamp when data has been received / sent
    DataSeq m_data; ///< Actual raw data
    PropertiesMap m_extraProperties; ///< Extra properties that can be used by other componets
};

/// @brief Pointer to @ref ToolsDataInfo
using ToolsDataInfoPtr = std::shared_ptr<ToolsDataInfo>;

/// @brief Dynamically allocate @ref ToolsDataInfo and return in in @ref ToolsDataInfoPtr;
CC_TOOLS_API ToolsDataInfoPtr makeDataInfo();

/// @brief Dynamically allocate @ref ToolsDataInfo and return in in @ref ToolsDataInfoPtr;
/// @details Automatically populate the timestamp.
CC_TOOLS_API ToolsDataInfoPtr makeDataInfoTimed();

}  // namespace cc_tools_qt

Q_DECLARE_METATYPE(cc_tools_qt::ToolsDataInfoPtr);



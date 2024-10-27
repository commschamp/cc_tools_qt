//
// Copyright 2018 - 2024 (C). Alex Robenko. All rights reserved.
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

/// @file
/// Contains version information of the library

#pragma once

#include "comms/version.h"

/// @brief Major verion of the library
#define CC_TOOLS_QT_MAJOR_VERSION 5U

/// @brief Minor verion of the library
#define CC_TOOLS_QT_MINOR_VERSION 3U

/// @brief Patch level of the library
#define CC_TOOLS_QT_PATCH_VERSION 4U

/// @brief Macro to create numeric version as single unsigned number
#define CC_TOOLS_QT_MAKE_VERSION(major_, minor_, patch_) \
    ((static_cast<unsigned>(major_) << 24) | \
     (static_cast<unsigned>(minor_) << 8) | \
     (static_cast<unsigned>(patch_)))

/// @brief Version of the COMMS library as single numeric value
#define CC_TOOLS_QT_VERSION CC_TOOLS_QT_MAKE_VERSION(CC_TOOLS_QT_MAJOR_VERSION, CC_TOOLS_QT_MINOR_VERSION, CC_TOOLS_QT_PATCH_VERSION)

namespace cc_tools_qt
{

/// @brief Major verion of the library
inline
constexpr unsigned versionMajor()
{
    return CC_TOOLS_QT_MAJOR_VERSION;
}

/// @brief Minor verion of the library
inline
constexpr unsigned versionMinor()
{
    return CC_TOOLS_QT_MINOR_VERSION;
}

/// @brief Patch level of the library
inline
constexpr unsigned versionPatch()
{
    return CC_TOOLS_QT_PATCH_VERSION;
}

/// @brief Create version of the library as single unsigned numeric value.
inline
constexpr unsigned versionCreate(unsigned major, unsigned minor, unsigned patch)
{
    return CC_TOOLS_QT_MAKE_VERSION(major, minor, patch);
}

/// @brief Version of the library as single numeric value
inline
constexpr unsigned version()
{
    return CC_TOOLS_QT_VERSION;
}

static_assert(COMMS_MAKE_VERSION(5, 3, 0) <= comms::version(),
    "The version of COMMS library is too old");

} // namespace cc_tools_qt

//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#ifdef WIN32

#ifdef CC_TOOLS_QT_EXPORT
#define CC_TOOLS_API __declspec(dllexport)
#else // #ifdef CC_TOOLS_QT_EXPORT
#define CC_TOOLS_API __declspec(dllimport)
#endif // #ifdef CC_TOOLS_QT_EXPORT

#else // #ifdef WIN32
#define CC_TOOLS_API
#endif // #ifdef WIN32


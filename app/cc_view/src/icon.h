//
// Copyright 2015 - 2026 (C). Alex Robenko. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later
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

#include <QtGui/QIcon>

namespace cc_tools_qt
{

namespace icon
{

[[nodiscard]] const QIcon& appIcon();
[[nodiscard]] const QIcon& add();
[[nodiscard]] const QIcon& addRaw();
[[nodiscard]] const QIcon& remove();
[[nodiscard]] const QIcon& clear();
[[nodiscard]] const QIcon& editClear();
[[nodiscard]] const QIcon& top();
[[nodiscard]] const QIcon& up();
[[nodiscard]] const QIcon& down();
[[nodiscard]] const QIcon& bottom();
[[nodiscard]] const QIcon& upload();
[[nodiscard]] const QIcon& save();
[[nodiscard]] const QIcon& saveAs();
[[nodiscard]] const QIcon& start();
[[nodiscard]] const QIcon& startAll();
[[nodiscard]] const QIcon& stop();
[[nodiscard]] const QIcon& edit();
[[nodiscard]] const QIcon& showRecv();
[[nodiscard]] const QIcon& showSent();
[[nodiscard]] const QIcon& pluginEdit();
[[nodiscard]] const QIcon& errorLog();
[[nodiscard]] const QIcon& connect();
[[nodiscard]] const QIcon& disconnect();
[[nodiscard]] const QIcon& comment();
[[nodiscard]] const QIcon& dup();
[[nodiscard]] const QIcon& filterEdit();
[[nodiscard]] const QIcon& filterApply();

}  // namespace icon

}  // namespace cc_tools_qt


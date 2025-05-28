//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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
#include "cc_tools_qt/version.h"

#include <QtCore/QString>
#include <QtCore/QVariantMap>

#include <list>
#include <utility>



namespace cc_tools_qt
{

class CC_TOOLS_API ToolsConfigMgr
{
public:

    ToolsConfigMgr();
    ~ToolsConfigMgr() noexcept;

    const QString& getLastFile() const;
    static const QString& getFilesFilter();

    QVariantMap loadConfig(const QString& filename);
    QVariantMap loadConfig(const QString& filename, bool updateAsLast);
    bool saveConfig(const QString& filename, const QVariantMap& config);
    bool saveConfig(const QString& filename, const QVariantMap& config, bool updateAsLast);

private:

    QString m_lastConfigFile;
};

}  // namespace cc_tools_qt



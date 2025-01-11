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

#include <utility>
#include <list>
#include <memory>

#include <QtCore/QString>
#include <QtCore/QVariantList>
#include <QtCore/QFile>

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsProtocol.h"

namespace cc_tools_qt
{

class CC_TOOLS_API ToolsMsgFileMgr
{
public:
    enum class Type
    {
        Recv,
        Send
    };

    ToolsMsgFileMgr();
    ~ToolsMsgFileMgr() noexcept;
    ToolsMsgFileMgr(const ToolsMsgFileMgr&);
    ToolsMsgFileMgr(ToolsMsgFileMgr&&);

    ToolsMsgFileMgr& operator=(const ToolsMsgFileMgr&);
    ToolsMsgFileMgr& operator=(ToolsMsgFileMgr&&);


    const QString& getLastFile() const;
    static const QString& getFilesFilter();

    ToolsMessagesList load(Type type, const QString& filename, ToolsProtocol& protocol);
    bool save(Type type, const QString& filename, const ToolsMessagesList& msgs);

    using FileSaveHandler = std::shared_ptr<QFile>;
    static FileSaveHandler startRecvSave(const QString& filename);
    static void addToRecvSave(FileSaveHandler handler, const ToolsMessage& msg, bool flush = false);
    static void flushRecvFile(FileSaveHandler handler);

private:
    QString m_lastFile;
};

}  // namespace cc_tools_qt



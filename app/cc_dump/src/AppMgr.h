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

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "cc_tools_qt/PluginMgr.h"
#include "cc_tools_qt/MsgMgr.h"
#include "cc_tools_qt/MsgFileMgr.h"
#include "cc_tools_qt/MsgSendMgr.h"

#include "CsvDumpMessageHandler.h"
#include "RecordMessageHandler.h"

namespace comms_dump
{

class AppMgr  : public QObject
{
    Q_OBJECT
public:
    struct Config
    {
        QString m_pluginsDir;
        QString m_pluginConfigFile;
        QString m_outMsgsFile;
        QString m_inMsgsFile;
        unsigned m_lastWait = 0U;
        bool m_recordOutgoing = false;
        bool m_quiet = false;
    };

    AppMgr();
    ~AppMgr() noexcept;

    bool start(const Config& config);

private slots:
    void flushOutput();

private:
    typedef cc_tools_qt::PluginMgr::ListOfPluginInfos ListOfPluginInfos;
    typedef std::unique_ptr<CsvDumpMessageHandler> CsvDumpMessageHandlerPtr;
    typedef std::unique_ptr<RecordMessageHandler> RecordMessageHandlerPtr;

    bool applyPlugins(const ListOfPluginInfos& plugins);
    void dispatchMsg(cc_tools_qt::Message& msg);

    cc_tools_qt::PluginMgr m_pluginMgr;
    cc_tools_qt::MsgMgr m_msgMgr;
    cc_tools_qt::MsgFileMgr m_msgFileMgr;
    cc_tools_qt::MsgSendMgr m_msgSendMgr;
    Config m_config;
    CsvDumpMessageHandlerPtr m_csvDump;
    RecordMessageHandlerPtr m_record;
    QTimer m_flushTimer;
};

} /* namespace comms_dump */

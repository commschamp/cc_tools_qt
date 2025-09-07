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

#include <iostream>

#include <QtWidgets/QApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>

#include "PluginMgrG.h"
#include "GuiAppMgr.h"

#include "widget/MainWindowWidget.h"
#include "icon.h"
#include "dir.h"

namespace
{

const QString CleanOptStr("clean");
const QString ConfigOptStr("config");
const QString PluginsOptStr("plugins");
const QString DebugOptStr("debug");

void metaTypesRegisterAll()
{
    qRegisterMetaType<cc_tools_qt::GuiAppMgr::ActionPtr>();
}

void initSingletons()
{
    static_cast<void>(cc_tools_qt::PluginMgrG::instanceRef());
    static_cast<void>(cc_tools_qt::MsgMgrG::instanceRef());
    static_cast<void>(cc_tools_qt::GuiAppMgr::instance());
}

void prepareCommandLineOptions(QCommandLineParser& parser)
{
    parser.addHelpOption();

    QCommandLineOption cleanOpt(
        CleanOptStr,
        QCoreApplication::translate("main", "Clean start.")
    );
    parser.addOption(cleanOpt);

    QCommandLineOption configOpt(
        QStringList() << "c" << ConfigOptStr,
        QCoreApplication::translate("main", "Load configuration file from \"config\" subdirectory. "
                                            "If not specified, \"default\" configuration is loaded."),
        QCoreApplication::translate("main", "config_name")
    );
    parser.addOption(configOpt);

    QCommandLineOption pluginsOpt(
        QStringList() << "p" << PluginsOptStr,
        QCoreApplication::translate("main", "Provide plugins configuration file."),
        QCoreApplication::translate("main", "filename")
    );
    parser.addOption(pluginsOpt);

    QCommandLineOption debugOpt(
        QStringList() << "d" << DebugOptStr,
        QCoreApplication::translate("main", "Debug output level. When 0 means no output"),
        QCoreApplication::translate("main", "value") + " (=0)",
        "0"
    );
    parser.addOption(debugOpt);
}

}  // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    metaTypesRegisterAll();
    initSingletons();

    QCommandLineParser parser;
    prepareCommandLineOptions(parser);
    parser.process(app);

    cc_tools_qt::MainWindowWidget window;
    window.setWindowIcon(cc_tools_qt::icon::appIcon());
    window.showMaximized();

    auto pluginsDir = cc_tools_qt::getPluginsDir();
    if (pluginsDir.isEmpty()) {
        std::cerr << "ERROR: Failed to find plugins directory!" << std::endl;
        return -1;
    }
    app.addLibraryPath(pluginsDir);

    auto& pluginMgr = cc_tools_qt::PluginMgrG::instanceRef();
    pluginMgr.setPluginsDir(pluginsDir);

    auto& guiAppMgr = cc_tools_qt::GuiAppMgr::instanceRef();
    guiAppMgr.setDebugOutputLevel(parser.value(DebugOptStr).toUInt());
    do {
        if (parser.isSet(CleanOptStr) && guiAppMgr.startClean()) {
            break;
        }

        if (guiAppMgr.startFromFile(parser.value(PluginsOptStr))) {
            break;
        }

        if (guiAppMgr.startFromConfig(parser.value(ConfigOptStr))) {
            break;
        }

        guiAppMgr.startClean();
    } while (false);

    QObject::connect(
        &app, &QCoreApplication::aboutToQuit,
        []()
        {
            cc_tools_qt::MsgMgrG::instanceRef().deleteAllMsgs();
        });

    auto retval = app.exec();
    return retval;
}


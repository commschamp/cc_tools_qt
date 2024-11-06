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


#include "GuiAppMgr.h"

#include <cassert>
#include <memory>

#include <QtCore/QTimer>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QList>

#include "cc_tools_qt/property/message.h"
#include "DefaultMessageDisplayHandler.h"
#include "PluginMgrG.h"
#include "MsgFileMgrG.h"
#include "dir.h"

#include <iostream>

namespace cc_tools_qt
{

namespace
{

const QString AppDataStorageFileName("startup_config.json");

QString getConfigPath(const QString& configName)
{
    QFileInfo fileInfo(configName);
    auto getPathFunc = 
        [&configName](const QDir& inDir)
        {
            if (configName.isEmpty()) {
                return inDir.absoluteFilePath("default.cfg");
            }

            return inDir.absoluteFilePath(configName + ".cfg");
        };

    //std::cout << "Config dir: " << getAppDataDir().toStdString() << std::endl;
    QFileInfo config1(getPathFunc(QDir(getAppDataDir())));
    if (config1.exists()) {
        return config1.absoluteFilePath();
    }

    return QFileInfo(getPathFunc(QDir(getConfigDir()))).absoluteFilePath();
}

}  // namespace

GuiAppMgr* GuiAppMgr::instance()
{
    return &(instanceRef());
}

GuiAppMgr& GuiAppMgr::instanceRef()
{
    static GuiAppMgr mgr;
    return mgr;
}

GuiAppMgr::~GuiAppMgr() noexcept = default;

bool GuiAppMgr::startClean()
{
    return true;
}

bool GuiAppMgr::startFromConfig(const QString& configName)
{
    return startFromFile(getConfigPath(configName));
}

bool GuiAppMgr::startFromFile(const QString& filename)
{
    if (filename.isEmpty()) {
        return false;
    }

    QFile configFile(filename);
    if (!configFile.exists()) {
        return false;
    }

    auto& pluginMgr = PluginMgrG::instanceRef();
    auto plugins = pluginMgr.loadPluginsFromConfigFile(filename);
    if (plugins.empty()) {
        return false;
    }

    return applyNewPlugins(plugins);
}

void GuiAppMgr::msgCommentUpdated(ToolsMessagePtr msg)
{
    assert(msg == m_clickedMsg);
    emit sigMsgCommentUpdated(std::move(msg));
}

const GuiAppMgr::FilteredMessages& GuiAppMgr::getFilteredMessages() const
{
    return m_filteredMessages;
}

void GuiAppMgr::setFilteredMessages(FilteredMessages&& filteredMessages)
{
    if (filteredMessages == m_filteredMessages) {
        return;
    }
    
    m_filteredMessages = std::move(filteredMessages);

    if (recvListApplyFilter()) {
        refreshRecvList();
    }
}

QString GuiAppMgr::messageDesc(const ToolsMessage& msg)
{
    return QString("(%1) %2").arg(msg.idAsString()).arg(msg.name());
}

void GuiAppMgr::setDebugOutputLevel(unsigned level)
{
    m_debugOutputLevel = level;
}

void GuiAppMgr::pluginsEditClicked()
{
    emit sigPluginsEditDialog();
}

void GuiAppMgr::recvStartClicked()
{
    MsgMgrG::instanceRef().setRecvEnabled(true);
    m_recvState = RecvState::Running;
    emitRecvStateUpdate();
}

void GuiAppMgr::recvStopClicked()
{
    MsgMgrG::instanceRef().setRecvEnabled(false);
    m_recvState = RecvState::Idle;
    emitRecvStateUpdate();
}

void GuiAppMgr::recvLoadClicked()
{
    emit sigLoadRecvMsgsDialog();
}

void GuiAppMgr::recvSaveClicked()
{
    emit sigSaveRecvMsgsDialog();
}

void GuiAppMgr::recvCommentClicked()
{
    assert(m_selType == SelectionType::Recv);
    assert(m_clickedMsg);
    emit sigMsgCommentDialog(m_clickedMsg);
}

void GuiAppMgr::recvDupClicked()
{
    assert(m_selType == SelectionType::Recv);
    assert(m_clickedMsg);
    auto prot = MsgMgrG::instanceRef().getProtocol();
    auto newMsg = prot->cloneMessage(*m_clickedMsg);
    sendAddNewMessage(std::move(newMsg));
}

void GuiAppMgr::recvDeleteClicked()
{
    assert(!recvListEmpty());
    assert(m_selType == SelectionType::Recv);
    assert(m_clickedMsg);

    MsgMgrG::instanceRef().deleteMsg(m_clickedMsg);

    clearDisplayedMessage();
    emit sigRecvDeleteSelectedMsg();
    decRecvListCount();
}

void GuiAppMgr::recvClearClicked()
{
    assert(0 < m_recvListCount);
    clearRecvList(true);
}

void GuiAppMgr::recvEditFilterClicked()
{
    emit sigRecvFilterDialog(MsgMgrG::instanceRef().getProtocol());
}

void GuiAppMgr::recvShowRecvToggled(bool checked)
{
    updateRecvListMode(RecvListMode_ShowReceived, checked);
}

void GuiAppMgr::recvShowSentToggled(bool checked)
{
    updateRecvListMode(RecvListMode_ShowSent, checked);
}

void GuiAppMgr::recvShowGarbageToggled(bool checked)
{
    updateRecvListMode(RecvListMode_ShowGarbage, checked);
}

void GuiAppMgr::recvApplyFilterToggled(bool checked)
{
    updateRecvListMode(RecvListMode_ApplyFilter, checked);
}

void GuiAppMgr::sendStartClicked()
{
    m_sendState = SendState::SendingSingle;
    emitSendStateUpdate();
}

void GuiAppMgr::sendStartAllClicked()
{
    m_sendState = SendState::SendingAll;
    emitSendStateUpdate();
}

void GuiAppMgr::sendStopClicked()
{
    m_sendState = SendState::Idle;
    m_sendMgr.stop();
    emitSendStateUpdate();
}

void GuiAppMgr::sendLoadClicked()
{
    emit sigLoadSendMsgsDialog(0 < m_sendListCount);
}

void GuiAppMgr::sendSaveClicked()
{
    emit sigSaveSendMsgsDialog();
}

void GuiAppMgr::sendAddClicked()
{
    emit sigNewSendMsgDialog(MsgMgrG::instanceRef().getProtocol());
}

void GuiAppMgr::sendAddRawClicked()
{
    emit sigSendRawMsgDialog(MsgMgrG::instanceRef().getProtocol());
}

void GuiAppMgr::sendEditClicked()
{
    assert(m_clickedMsg);
    emit sigUpdateSendMsgDialog(m_clickedMsg, MsgMgrG::instanceRef().getProtocol());
}

void GuiAppMgr::sendCommentClicked()
{
    assert(m_selType == SelectionType::Send);
    assert(m_clickedMsg);
    emit sigMsgCommentDialog(m_clickedMsg);
}

void GuiAppMgr::sendDupClicked()
{
    assert(m_selType == SelectionType::Send);
    assert(m_clickedMsg);
    auto prot = MsgMgrG::instanceRef().getProtocol();
    auto newMsg = prot->cloneMessage(*m_clickedMsg);
    sendAddNewMessage(std::move(newMsg));
}

void GuiAppMgr::sendDeleteClicked()
{
    assert(!sendListEmpty());
    assert(m_selType == SelectionType::Send);
    assert(m_clickedMsg);

    clearDisplayedMessage();
    emit sigSendDeleteSelectedMsg();

    decSendListCount();
}

void GuiAppMgr::sendClearClicked()
{
    emit sigSendClear();
    assert(0 < m_sendListCount);
    bool wasSelected = (m_selType == SelectionType::Send);
    assert((!wasSelected) || (m_clickedMsg));

    m_sendListCount = 0;

    if (wasSelected) {
        clearDisplayedMessage();
        emitSendNotSelected();
    }

    emit sigSendListCountReport(m_sendListCount);
}

void GuiAppMgr::sendTopClicked()
{
    emit sigSendMoveSelectedTop();
}

void GuiAppMgr::sendUpClicked()
{
    emit sigSendMoveSelectedUp();
}

void GuiAppMgr::sendDownClicked()
{
    emit sigSendMoveSelectedDown();
}

void GuiAppMgr::sendBottomClicked()
{
    emit sigSendMoveSelectedBottom();
}

void GuiAppMgr::recvMsgClicked(ToolsMessagePtr msg, int idx)
{
    emit sigSendMsgListClearSelection();
    emitSendNotSelected();

    msgClicked(msg, SelectionType::Recv);
    if (!m_clickedMsg) {
        emit sigRecvMsgListClearSelection();
        emitRecvNotSelected();
    }
    else {
        emit sigRecvMsgSelected(idx);
    }
}

void GuiAppMgr::sendMsgClicked(ToolsMessagePtr msg, int idx)
{
    emit sigRecvMsgListClearSelection();
    emitRecvNotSelected();

    msgClicked(msg, SelectionType::Send);
    if (!m_clickedMsg) {
        emit sigSendMsgListClearSelection();
        emitSendNotSelected();
    }
    else {
        emit sigSendMsgSelected(idx);
    }
}

void GuiAppMgr::sendMsgDoubleClicked(ToolsMessagePtr msg, int idx)
{
    // Equivalent to selection + edit
    assert(msg);
    if (msg != m_clickedMsg) {
        sendMsgClicked(msg, idx);
    }
    assert(m_clickedMsg == msg);
    sendEditClicked();
}

void GuiAppMgr::sendSelectedMsgMoved(int idx)
{
    assert(0 <= idx);
    assert(m_clickedMsg);
    assert(m_selType == SelectionType::Send);
    emit sigSendMsgSelected(idx);
}

void GuiAppMgr::addMainToolbarAction(ActionPtr action)
{
    emit sigAddMainToolbarAction(std::move(action));
}

void GuiAppMgr::connectSocketClicked()
{
    auto socket = MsgMgrG::instanceRef().getSocket();
    assert(socket);
    [[maybe_unused]] bool connected = socket->socketConnect();
}

void GuiAppMgr::disconnectSocketClicked()
{
    auto socket = MsgMgrG::instanceRef().getSocket();
    assert(socket);
    socket->socketDisconnect();
}

GuiAppMgr::RecvState GuiAppMgr::recvState() const
{
    return m_recvState;
}

bool GuiAppMgr::recvMsgListSelectOnAddEnabled()
{
    return m_recvListSelectOnAdd;
}

bool GuiAppMgr::recvListEmpty() const
{
    return m_recvListCount == 0;
}

void GuiAppMgr::recvLoadMsgsFromFile(const QString& filename)
{
    auto& msgMgr = MsgMgrG::instanceRef();
    auto msgs = MsgFileMgrG::instanceRef().load(ToolsMsgFileMgr::Type::Recv, filename, *msgMgr.getProtocol());

    clearRecvList(false);
    msgMgr.deleteAllMsgs();

    msgMgr.addMsgs(msgs);
}

void GuiAppMgr::recvSaveMsgsToFile(const QString& filename)
{
    emit sigRecvSaveMsgs(filename);
}

bool GuiAppMgr::recvListShowsReceived() const
{
    return (m_recvListMode & RecvListMode_ShowReceived) != 0;
}

bool GuiAppMgr::recvListShowsSent() const
{
    return (m_recvListMode & RecvListMode_ShowSent) != 0;
}

bool GuiAppMgr::recvListShowsGarbage() const
{
    return (m_recvListMode & RecvListMode_ShowGarbage) != 0;
}

bool GuiAppMgr::recvListApplyFilter() const
{
    return (m_recvListMode & RecvListMode_ApplyFilter) != 0;
}

unsigned GuiAppMgr::recvListModeMask() const
{
    return m_recvListMode;
}

GuiAppMgr::SendState GuiAppMgr::sendState() const
{
    return m_sendState;
}

void GuiAppMgr::sendAddNewMessage(ToolsMessagePtr msg)
{
    ++m_sendListCount;
    emit sigSendListCountReport(m_sendListCount);
    emit sigAddSendMsg(msg);

    sendMsgClicked(msg, static_cast<int>(m_sendListCount) - 1);
    assert(m_selType == SelectionType::Send);
    assert(m_clickedMsg);
}

void GuiAppMgr::sendUpdateMessage(ToolsMessagePtr msg)
{
    assert(!sendListEmpty());
    assert(msg);
    assert(m_clickedMsg);
    m_clickedMsg = msg;
    emit sigSendMsgUpdated(msg);
    displayMessage(std::move(msg));
}

bool GuiAppMgr::sendListEmpty() const
{
    return m_sendListCount == 0;
}

void GuiAppMgr::sendLoadMsgsFromFile(bool clear, const QString& filename)
{
    emit sigSendLoadMsgs(clear, filename, MsgMgrG::instanceRef().getProtocol());
}

void GuiAppMgr::sendSaveMsgsToFile(const QString& filename)
{
    emit sigSendSaveMsgs(filename);
}

void GuiAppMgr::sendUpdateList(const MessagesList& msgs)
{
    decltype(m_clickedMsg) clickedMsg;
    if (m_selType == SelectionType::Send) {
        assert(m_clickedMsg);
        assert(0 < m_sendListCount);
        clickedMsg = m_clickedMsg;
        sendMsgClicked(m_clickedMsg, -1);
        assert(!m_clickedMsg);
    }

    int clickedIdx = 0;
    for (auto& m : msgs) {
        if (m == clickedMsg) {
            break;
        }
        ++clickedIdx;
    }

    m_sendListCount = static_cast<decltype(m_sendListCount)>(msgs.size());
    emit sigSendListCountReport(m_sendListCount);
    if ((clickedMsg) && (static_cast<std::size_t>(clickedIdx) < msgs.size())) {
        sendMsgClicked(clickedMsg, clickedIdx);
    }
}

void GuiAppMgr::deleteMessages(MessagesList&& msgs)
{
    auto& msgMgr = MsgMgrG::instanceRef();
    if (msgMgr.getAllMsgs().size() == msgs.size()) {
        msgMgr.deleteAllMsgs();
        return;
    }

    msgMgr.deleteMsgs(msgs);
}

void GuiAppMgr::sendMessages(MessagesList&& msgs)
{
    m_sendMgr.start(MsgMgrG::instanceRef().getProtocol(), std::move(msgs));
}

GuiAppMgr::ActivityState GuiAppMgr::getActivityState()
{
    auto& pluginMgr = PluginMgrG::instanceRef();
    if (!pluginMgr.hasAppliedPlugins()) {
        return ActivityState::Inactive;
    }

    return ActivityState::Active;
}

bool GuiAppMgr::applyNewPlugins(const ListOfPluginInfos& plugins)
{
    auto& pluginMgr = PluginMgrG::instanceRef();
    auto& msgMgr = MsgMgrG::instanceRef();

    emit sigClearAllMainToolbarActions();
    bool hasApplied = pluginMgr.hasAppliedPlugins();
    bool needsReload = pluginMgr.needsReload(plugins);

    auto currSocket = msgMgr.getSocket();
    if (currSocket) {
        currSocket->socketDisconnect();
        currSocket.reset();
    }

    if (0U < m_sendListCount) {
        bool protocolChanging = pluginMgr.isProtocolChanging(plugins);
        if (protocolChanging) {
            sendClearClicked();
        }
    }

    if (hasApplied) {
        if (needsReload) {
            clearRecvList(false);
            msgMgr.deleteAllMsgs();
        }

        msgMgr.stop();
        msgMgr.clear();
        emit sigActivityStateChanged(static_cast<int>(ActivityState::Inactive));
    }

    if (needsReload) {
        assert(hasApplied);
        ListOfPluginInfos pluginsToUnload;
        auto& availablePlugins = pluginMgr.getAvailablePlugins();
        std::copy_if(
            availablePlugins.begin(), availablePlugins.end(), std::back_inserter(pluginsToUnload),
            [&plugins](const ToolsPluginMgr::PluginInfoPtr& ptr) -> bool
            {
                auto iter =
                    std::find(plugins.begin(), plugins.end(), ptr);
                return iter == plugins.end();
            });

        for (auto& ptr : pluginsToUnload) {
            pluginMgr.unloadAppliedPlugin(*ptr);
        }
        emit sigActivityStateChanged(static_cast<int>(ActivityState::Clear));
    }

    typedef QList<ActionPtr> ListOfGuiActions;

    struct ApplyInfo
    {
        ToolsSocketPtr m_socket;
        QList<ToolsFilterPtr> m_filters;
        ToolsProtocolPtr m_protocol;
        ListOfGuiActions m_actions;
    };

    auto applyInfo = ApplyInfo();
    for (auto& info : plugins) {
        auto* plugin = pluginMgr.loadPlugin(*info);
        if (plugin == nullptr) {
            [[maybe_unused]] static constexpr bool Failed_to_load_plugin = false;
            assert(Failed_to_load_plugin);
            continue;
        }

        plugin->setDebugOutputLevel(m_debugOutputLevel);

        if (!applyInfo.m_socket) {
            applyInfo.m_socket = plugin->createSocket();
        }

        applyInfo.m_filters.append(plugin->createFilter());

        if (!applyInfo.m_protocol) {
            applyInfo.m_protocol = plugin->createProtocol();
        }

        auto guiActions = plugin->createGuiActions();
        for (auto* action : guiActions) {
            applyInfo.m_actions.append(ActionPtr(action));
        }
    }

    if (!applyInfo.m_socket) {
        std::cerr << "Socket hasn't been set!" << std::endl;
        return false;
    }

    if (!applyInfo.m_protocol) {
        std::cerr << "Protocol hasn't been set!" << std::endl;
        return false;
    }

    auto connectProps = applyInfo.m_socket->connectionProperties();
    bool socketAutoConnect =
        (connectProps & ToolsSocket::ConnectionProperty_Autoconnect) != 0U;
    bool socketNonDisconnectable =
        (connectProps & ToolsSocket::ConnectionProperty_NonDisconnectable) != 0U;

    msgMgr.setSocket(std::move(applyInfo.m_socket));

    for (auto& filter : applyInfo.m_filters) {
        msgMgr.addFilter(std::move(filter));
    }

    msgMgr.setProtocol(std::move(applyInfo.m_protocol));

    msgMgr.start();
    emit sigActivityStateChanged(static_cast<int>(ActivityState::Active));

    for (auto& action : applyInfo.m_actions) {
        emit sigAddMainToolbarAction(std::move(action));
    }

    pluginMgr.setAppliedPlugins(plugins);

    bool connectDisabled = socketAutoConnect && socketNonDisconnectable;
    emit sigSocketConnectEnabled(!connectDisabled);

    if (socketAutoConnect) {
        msgMgr.getSocket()->socketConnect();
    }
    return true;
}

GuiAppMgr::GuiAppMgr(QObject* parentObj)
  : Base(parentObj)
{
    m_pendingDisplayTimer.setSingleShot(true);

    connect(
        &m_pendingDisplayTimer, SIGNAL(timeout()),
        this, SLOT(pendingDisplayTimeout()));

    m_sendMgr.setSendMsgsCallbackFunc(
        [](MessagesList&& msgsToSend)
        {
            MsgMgrG::instanceRef().sendMsgs(std::move(msgsToSend));
        });

    m_sendMgr.setSendCompeteCallbackFunc(
        [this]()
        {
            sendStopClicked();
        });

    auto& msgMgr = MsgMgrG::instanceRef();
    msgMgr.setMsgAddedCallbackFunc(
        [this](ToolsMessagePtr msg)
        {
            msgAdded(std::move(msg));
        });

    msgMgr.setErrorReportCallbackFunc(
        [this](const QString& error)
        {
            errorReported(error);
        });

    msgMgr.setSocketConnectionStatusReportCallbackFunc(
        [this](bool connected)
        {
            emit sigSocketConnected(connected);
        });

    refreshRecvState();
}

void GuiAppMgr::emitRecvStateUpdate()
{
    emit sigSetRecvState(static_cast<int>(m_recvState));
}

void GuiAppMgr::emitSendStateUpdate()
{
    emit sigSetSendState(static_cast<int>(m_sendState));
}

void GuiAppMgr::msgAdded(ToolsMessagePtr msg)
{
    assert(msg);
    auto type = property::message::Type().getFrom(*msg);
    assert((type == MsgType::Received) || (type == MsgType::Sent));

#ifndef NDEBUG

    static const char* const RecvPrefix = "<-- ";
    static const char* const SentPrefix = "--> ";

    const char* prefix = RecvPrefix;
    if (type == MsgType::Sent) {
        prefix = SentPrefix;
    }

    std::cout << '[' << property::message::Timestamp().getFrom(*msg) << "] " << prefix << msg->name() << std::endl;
#endif

    if (!canAddToRecvList(*msg, type)) {
        return;
    }

    addMsgToRecvList(msg);

    if (m_clickedMsg) {
        return;
    }

    if (m_pendingDisplayWaitInProgress) {
        m_pendingDisplayMsg = std::move(msg);
        return;
    }

    displayMessage(std::move(msg));

    static const int DisplayTimeout = 250;
    m_pendingDisplayWaitInProgress = true;
    m_pendingDisplayTimer.start(DisplayTimeout);
}

void GuiAppMgr::errorReported(const QString& msg)
{
    emit sigErrorReported(msg + tr("\nThe tool may not work properly!"));
}

void GuiAppMgr::pendingDisplayTimeout()
{
    m_pendingDisplayWaitInProgress = false;
    if (m_pendingDisplayMsg) {
        displayMessage(std::move(m_pendingDisplayMsg));
    }
}

void GuiAppMgr::msgClicked(ToolsMessagePtr msg, SelectionType selType)
{
    assert(msg);
    if (m_clickedMsg == msg) {
        assert(selType == m_selType);
        clearDisplayedMessage();
        emit sigRecvMsgListSelectOnAddEnabled(true);
        return;
    }

    m_selType = selType;
    m_clickedMsg = msg;
    displayMessage(m_clickedMsg);
    emit sigRecvMsgListSelectOnAddEnabled(false);
}

void GuiAppMgr::displayMessage(ToolsMessagePtr msg)
{
    m_pendingDisplayMsg.reset();
    emit sigDisplayMsg(msg);
}

void GuiAppMgr::clearDisplayedMessage()
{
    m_selType = SelectionType::None;
    m_clickedMsg.reset();
    emit sigClearDisplayedMsg();
}

void GuiAppMgr::refreshRecvList()
{
    auto clickedMsg = m_clickedMsg;
    if (m_selType == SelectionType::Recv) {
        assert(m_clickedMsg);
        assert(0 < m_recvListCount);
        recvMsgClicked(m_clickedMsg, static_cast<int>(m_recvListCount) - 1);
        assert(!m_clickedMsg);
    }
    else if (m_selType != SelectionType::Send) {
        emit sigClearDisplayedMsg();
    }

    clearRecvList(false);

    auto& allMsgs = MsgMgrG::instanceRef().getAllMsgs();
    for (auto& msg : allMsgs) {
        assert(msg);
        auto type = property::message::Type().getFrom(*msg);

        if (canAddToRecvList(*msg, type)) {
            addMsgToRecvList(msg);
            if (msg == clickedMsg) {
                assert(0 < m_recvListCount);
                recvMsgClicked(msg, static_cast<int>(m_recvListCount) - 1);
            }
        }
    }

    if (!m_clickedMsg) {
        emit sigRecvMsgListClearSelection();
    }
}

void GuiAppMgr::addMsgToRecvList(ToolsMessagePtr msg)
{
    assert(msg);
    ++m_recvListCount;
    emit sigRecvListCountReport(m_recvListCount);
    emit sigAddRecvMsg(msg);
}

void GuiAppMgr::clearRecvList(bool reportDeleted)
{
    bool wasSelected = (m_selType == SelectionType::Recv);
    bool sendSelected = (m_selType == SelectionType::Send);
    assert((!wasSelected) || (m_clickedMsg));
    assert((!sendSelected) || (m_clickedMsg));

    m_recvListCount = 0;

    if (!sendSelected) {
        clearDisplayedMessage();
    }

    if (wasSelected) {
        emit sigRecvMsgListSelectOnAddEnabled(true);
        emitRecvNotSelected();
    }

    emit sigRecvListCountReport(m_recvListCount);
    emit sigRecvClear(reportDeleted);
}

bool GuiAppMgr::canAddToRecvList(
    const ToolsMessage& msg,
    MsgType type) const
{
    assert((type == MsgType::Received) || (type == MsgType::Sent));

    if (msg.idAsString().isEmpty()) {
        assert(type == MsgType::Received);
        return recvListShowsGarbage();
    }

    if ((type == MsgType::Sent) && (!recvListShowsSent())) {
        return false;
    }

    if ((type == MsgType::Received) && (!recvListShowsReceived())) {
        return false;
    }

    if (!recvListApplyFilter()) {
        return true;
    }

    auto desc = messageDesc(msg);
    auto iter = std::lower_bound(m_filteredMessages.begin(), m_filteredMessages.end(), desc);
    return (iter == m_filteredMessages.end()) || (*iter != desc);
}

void GuiAppMgr::decRecvListCount()
{
    --m_recvListCount;
    if (recvListEmpty()) {
        emitRecvNotSelected();
    }
    emit sigRecvListCountReport(m_recvListCount);
}

void GuiAppMgr::decSendListCount()
{
    --m_sendListCount;
    if (sendListEmpty()) {
        emitSendNotSelected();
    }
    emit sigSendListCountReport(m_sendListCount);
}

void GuiAppMgr::emitRecvNotSelected()
{
    emit sigRecvMsgSelected(-1);
}

void GuiAppMgr::emitSendNotSelected()
{
    emit sigSendMsgSelected(-1);
}

void GuiAppMgr::updateRecvListMode(RecvListMode mode, bool checked)
{
    auto mask =
        static_cast<decltype(m_recvListMode)>(mode);
    if (checked) {
        m_recvListMode |= mask;
    }
    else {
        m_recvListMode &= static_cast<decltype(m_recvListMode)>(~mask);
    }

    if (mode != RecvListMode_ShowGarbage) {
        emit sigRecvListTitleNeedsUpdate();
    }
    refreshRecvList();
}

void GuiAppMgr::refreshRecvState()
{
    if (m_recvState == RecvState::Running) {
        recvStartClicked();
        return;
    }

    assert(m_recvState == RecvState::Idle);
    recvStopClicked();
}

}  // namespace cc_tools_qt


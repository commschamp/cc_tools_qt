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

#include "cc_tools_qt/ToolsMsgFileMgr.h"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <iostream>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QVariantMap>

#include "cc_tools_qt/property/message.h"

namespace cc_tools_qt
{

namespace
{

class IdProp : public property::message::ToolsMsgPropBase<QString>
{
    using Base = property::message::ToolsMsgPropBase<QString>;
public:
    IdProp() : Base("id") {}
};

class MsgIdxProp : public property::message::ToolsMsgPropBase<unsigned>
{
    using Base = property::message::ToolsMsgPropBase<unsigned>;

public:
    MsgIdxProp() : Base("msg_idx") {}
};

class DataProp : public property::message::ToolsMsgPropBase<QString>
{
    using Base = property::message::ToolsMsgPropBase<QString>;

public:
    DataProp() : Base("data") {}
};

class DelayProp : public property::message::ToolsMsgPropBase<unsigned long long>
{
    using Base = property::message::ToolsMsgPropBase<unsigned long long>;
public:
    DelayProp() : Base("delay") {}
};

class DelayUnitsProp : public property::message::ToolsMsgPropBase<QString>
{
    using Base = property::message::ToolsMsgPropBase<QString>;
public:
    DelayUnitsProp() : Base("delay_units") {}
};

class RepeatProp : public property::message::ToolsMsgPropBase<unsigned long long>
{
    using Base = property::message::ToolsMsgPropBase<unsigned long long>;
public:
    RepeatProp() : Base("repeat") {}
};

class RepeatUnitsProp : public property::message::ToolsMsgPropBase<QString>
{
    using Base = property::message::ToolsMsgPropBase<QString>;
public:
    RepeatUnitsProp() : Base("repeat_units") {}
};

class RepeatCountProp : public property::message::ToolsMsgPropBase<unsigned>
{
    using Base = property::message::ToolsMsgPropBase<unsigned>;
public:
    RepeatCountProp() : Base("repeat_count") {}
};

class CommentProp : public property::message::ToolsMsgPropBase<QString>
{
    using Base = property::message::ToolsMsgPropBase<QString>;
public:
    CommentProp() : Base("comment") {}
};

class TimestampProp : public property::message::ToolsMsgPropBase<unsigned long long>
{
    using Base = property::message::ToolsMsgPropBase<unsigned long long>;
public:
    TimestampProp() : Base("timestamp") {}
};

class TypeProp : public property::message::ToolsMsgPropBase<unsigned>
{
    using Base = property::message::ToolsMsgPropBase<unsigned>;
public:
    TypeProp() : Base("type") {}
};

class ExtraPropsProp : public property::message::ToolsMsgPropBase<QVariantMap>
{
    using Base = property::message::ToolsMsgPropBase<QVariantMap>;
public:
    ExtraPropsProp() : Base("extra_info") {}
};

QString encodeMsgData(const ToolsMessage& msg)
{
    ToolsMessage::DataSeq msgData;
    do {
        if (!msg.idAsString().isEmpty()) {
            msgData = msg.encodeData();
            break;
        }

        auto rawDataMsg = property::message::ToolsMsgRawDataMsg().getFrom(msg);
        if (!rawDataMsg) {
            break;
        }

        msgData = rawDataMsg->encodeData();
    } while (false);

    QString msgDataStr;
    for (auto dataByte : msgData) {
        if (!msgDataStr.isEmpty()) {
            msgDataStr.append(' ');
        }
        msgDataStr.append(
            QString("%1").arg(
                static_cast<unsigned>(dataByte), 2, 16, QChar('0')));
    }

    return msgDataStr;
}

ToolsMessagePtr createMsgObjectFrom(
    const QVariant& msgMapVar,
    ToolsProtocol& protocol)
{
    if ((!msgMapVar.isValid()) || (!msgMapVar.canConvert<QVariantMap>())) {
        return ToolsMessagePtr();
    }

    auto msgMap = msgMapVar.value<QVariantMap>();
    auto msgId = IdProp().getFrom(msgMap);
    auto msgIdx = MsgIdxProp().getFrom(msgMap);
    auto dataStr = DataProp().getFrom(msgMap);

    if (msgId.isEmpty() && dataStr.isEmpty()) {
        return ToolsMessagePtr();
    }

    QString stripedDataStr;
    stripedDataStr.reserve(dataStr.size());
    std::copy_if(
        dataStr.begin(), dataStr.end(), std::back_inserter(stripedDataStr),
        [](QChar ch) -> bool
        {
            return
                ch.isDigit() ||
                ((QChar('A') <= ch) && (ch <= QChar('F'))) ||
                ((QChar('a') <= ch) && (ch <= QChar('f')));
        });

    if ((stripedDataStr.size() & 0x1) != 0) {
        stripedDataStr.prepend(QChar('0'));
    }

    ToolsMessage::DataSeq data;
    QString num;
    for (auto ch : stripedDataStr) {
        num.append(ch);
        if (num.size() == 1) {
            continue;
        }

        assert(num.size() == 2);
        bool ok = false;
        auto byte = num.toInt(&ok, 16);
        assert(ok);
        assert((0 <= byte) && (byte <= 0xff));
        data.push_back(static_cast<ToolsMessage::DataSeq::value_type>(byte));
        num.clear();
    }

    auto extraInfo = ExtraPropsProp().getFrom(msgMap);

    ToolsMessagePtr msg;
    if (msgId.isEmpty()) {
        msg = protocol.createInvalidMessage(data);
        if (!msg) {
            return msg;
        }

        if (!extraInfo.isEmpty()) {
            property::message::ToolsMsgExtraInfo().setTo(std::move(extraInfo), *msg);
            protocol.updateMessage(*msg);
        }
        return msg;
    }

    do {
        if (msgIdx == 0U) {
            break;
        }

        msg = protocol.createMessage(msgId, msgIdx);
        if (!msg) {
            break;
        }

        if (!msg->decodeData(data)) {
            msg.reset();
            break;
        }

    } while (false);

    unsigned idx = 0;
    while (!msg) {
        msg = protocol.createMessage(msgId, idx);
        if (!msg) {
            break;
        }

        ++idx;
        if (msg->decodeData(data)) {
            break;
        }

        msg.reset();
    }

    if (msg) {
        if (!extraInfo.isEmpty()) {
            property::message::ToolsMsgExtraInfo().setTo(std::move(extraInfo), *msg);
        }

        protocol.updateMessage(*msg);
    }

    return msg;
}

QVariantMap convertRecvMsg(const ToolsMessage& msg)
{
    QVariantMap msgInfoMap;
    auto idStr = msg.idAsString();
    auto dataStr = encodeMsgData(msg);
    if (idStr.isEmpty() && dataStr.isEmpty()) {
        return msgInfoMap;
    }

    if (!idStr.isEmpty()) {
        IdProp().setTo(std::move(idStr), msgInfoMap);
        MsgIdxProp().setTo(property::message::ToolsMsgIdx().getFrom(msg), msgInfoMap);
    }

    DataProp().setTo(std::move(dataStr), msgInfoMap);
    TimestampProp().setTo(property::message::ToolsMsgTimestamp().getFrom(msg), msgInfoMap);
    TypeProp().setTo(static_cast<unsigned>(property::message::ToolsMsgType().getFrom(msg)), msgInfoMap);

    auto comment = property::message::ToolsMsgComment().getFrom(msg);
    if (!comment.isEmpty()) {
        CommentProp().setTo(comment, msgInfoMap);
    }

    auto extraInfo = property::message::ToolsMsgExtraInfo().getFrom(msg);
    if (!extraInfo.isEmpty()) {
        ExtraPropsProp().setTo(std::move(extraInfo), msgInfoMap);
    }

    return msgInfoMap;
}

QVariantList convertRecvMsgList(
    const ToolsMessagesList& allMsgs)
{
    QVariantList convertedList;
    for (auto& msg : allMsgs) {
        if (!msg) {
            [[maybe_unused]] static constexpr bool Message_must_exist = false;
            assert(Message_must_exist);
            continue;
        }

        QVariantMap msgInfoMap = convertRecvMsg(*msg);
        if (msgInfoMap.isEmpty()) {
            continue;
        }

        convertedList.append(QVariant::fromValue(msgInfoMap));
    }
    return convertedList;
}

ToolsMessagesList convertRecvMsgList(
    const QVariantList& msgs,
    ToolsProtocol& protocol)
{
    ToolsMessagesList convertedList;

    for (auto& msgMapVar : msgs) {
        auto msg = createMsgObjectFrom(msgMapVar, protocol);
        if (!msg) {
            continue;
        }

        assert(msgMapVar.isValid() && msgMapVar.canConvert<QVariantMap>());

        auto msgMap = msgMapVar.value<QVariantMap>();
        auto timestamp = TimestampProp().getFrom(msgMap);
        if (timestamp == 0) {
            // Not a receive list, skip message
            continue;
        }

        auto type = static_cast<ToolsMessage::Type>(TypeProp().getFrom(msgMap));
        auto comment = CommentProp().getFrom(msgMap);

        property::message::ToolsMsgTimestamp().setTo(timestamp, *msg);
        property::message::ToolsMsgType().setTo(type, *msg);
        property::message::ToolsMsgComment().setTo(comment, *msg);

        convertedList.push_back(std::move(msg));
    }
    return convertedList;
}

QVariantList convertSendMsgList(
    const ToolsMessagesList& allMsgs)
{
    QVariantList convertedList;
    for (auto& msg : allMsgs) {
        if (!msg) {
            [[maybe_unused]] static constexpr bool Message_must_exist = false;
            assert(Message_must_exist);
            continue;
        }

        QVariantMap msgInfoMap;
        IdProp().setTo(msg->idAsString(), msgInfoMap);
        MsgIdxProp().setTo(property::message::ToolsMsgIdx().getFrom(*msg), msgInfoMap);
        DataProp().setTo(encodeMsgData(*msg), msgInfoMap);
        DelayProp().setTo(property::message::ToolsMsgDelay().getFrom(*msg), msgInfoMap);
        DelayUnitsProp().setTo(property::message::ToolsMsgDelayUnits().getFrom(*msg), msgInfoMap);
        RepeatProp().setTo(property::message::ToolsMsgRepeatDuration().getFrom(*msg), msgInfoMap);
        RepeatUnitsProp().setTo(property::message::ToolsMsgRepeatDurationUnits().getFrom(*msg), msgInfoMap);
        RepeatCountProp().setTo(property::message::ToolsMsgRepeatCount().getFrom(*msg, 1U), msgInfoMap);

        auto comment = property::message::ToolsMsgComment().getFrom(*msg);
        if (!comment.isEmpty()) {
            CommentProp().setTo(comment, msgInfoMap);
        }

        auto extraInfo = property::message::ToolsMsgExtraInfo().getFrom(*msg);
        if (!extraInfo.isEmpty()) {
            ExtraPropsProp().setTo(std::move(extraInfo), msgInfoMap);
        }

        convertedList.append(QVariant::fromValue(msgInfoMap));
    }
    return convertedList;
}

ToolsMessagesList convertSendMsgList(
    const QVariantList& msgs,
    ToolsProtocol& protocol)
{
    ToolsMessagesList convertedList;
    unsigned long long prevTimestamp = 0;

    for (auto& msgMapVar : msgs) {
        auto msg = createMsgObjectFrom(msgMapVar, protocol);
        if (!msg) {
            continue;
        }

        assert(msgMapVar.isValid() && msgMapVar.canConvert<QVariantMap>());

        auto msgMap = msgMapVar.value<QVariantMap>();
        auto delay = DelayProp().getFrom(msgMap);
        auto delayUnits = DelayUnitsProp().getFrom(msgMap);
        auto repeatDuration = RepeatProp().getFrom(msgMap);
        auto repeatDurationUnits = RepeatUnitsProp().getFrom(msgMap);
        auto repeatCount = RepeatCountProp().getFrom(msgMap);
        auto comment = CommentProp().getFrom(msgMap);

        if ((repeatDuration == 0) && (repeatCount == 0)) {
            repeatCount = 1;

            do {
                if (delay != 0) {
                    break;
                }

                // Probably receive list is loaded
                auto timestamp = TimestampProp().getFrom(msgMap);
                if (timestamp == 0) {
                    break;
                }

                if (prevTimestamp == 0) {
                    prevTimestamp = timestamp;
                }

                auto delayTmp = timestamp - prevTimestamp;
                if (delayTmp <= 0) {
                    break;
                }

                prevTimestamp = timestamp;
                delay = delayTmp;
            } while (false);
        }

        property::message::ToolsMsgDelay().setTo(delay, *msg);
        property::message::ToolsMsgDelayUnits().setTo(std::move(delayUnits), *msg);
        property::message::ToolsMsgRepeatDuration().setTo(repeatDuration, *msg);
        property::message::ToolsMsgRepeatDurationUnits().setTo(std::move(repeatDurationUnits), *msg);
        property::message::ToolsMsgRepeatCount().setTo(repeatCount, *msg);
        property::message::ToolsMsgComment().setTo(comment, *msg);

        convertedList.push_back(std::move(msg));
    }
    return convertedList;
}

QVariantList convertMsgList(
    ToolsMsgFileMgr::Type type,
    const ToolsMessagesList& allMsgs)
{
    if (type == ToolsMsgFileMgr::Type::Recv) {
        return convertRecvMsgList(allMsgs);
    }

    return convertSendMsgList(allMsgs);
}

ToolsMessagesList convertMsgList(
    ToolsMsgFileMgr::Type type,
    const QVariantList& msgs,
    ToolsProtocol& protocol)
{
    if (type == ToolsMsgFileMgr::Type::Recv) {
        return convertRecvMsgList(msgs, protocol);
    }

    return convertSendMsgList(msgs, protocol);
}

}  // namespace

ToolsMsgFileMgr::ToolsMsgFileMgr() = default;
ToolsMsgFileMgr::~ToolsMsgFileMgr() noexcept = default;
ToolsMsgFileMgr::ToolsMsgFileMgr(const ToolsMsgFileMgr&) = default;
ToolsMsgFileMgr::ToolsMsgFileMgr(ToolsMsgFileMgr&&) = default;
ToolsMsgFileMgr& ToolsMsgFileMgr::operator=(const ToolsMsgFileMgr&) = default;
ToolsMsgFileMgr& ToolsMsgFileMgr::operator=(ToolsMsgFileMgr&&) = default;

const QString& ToolsMsgFileMgr::getLastFile() const
{
    return m_lastFile;
}

ToolsMessagesList ToolsMsgFileMgr::load(
    Type type,
    const QString& filename,
    ToolsProtocol& protocol)
{
    ToolsMessagesList allMsgs;
    do {
        QFile msgsFile(filename);
        if (!msgsFile.open(QIODevice::ReadOnly)) {
            std::cerr << "ERROR: Failed to load the file " <<
                filename.toStdString() << std::endl;
            break;
        }

        auto data = msgsFile.readAll();

        auto jsonError = QJsonParseError();
        auto jsonDoc = QJsonDocument::fromJson(data, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            std::cerr << "ERROR: Invalid contents of messages file!" << std::endl;
            break;
        }

        if (!jsonDoc.isArray()) {
            std::cerr << "ERROR: Invalid contents of messages file!" << std::endl;
            break;
        }

        auto topArray = jsonDoc.array();
        auto varList = topArray.toVariantList();
        allMsgs = convertMsgList(type, varList, protocol);
        m_lastFile = filename;
    } while (false);

    return allMsgs;
}

bool ToolsMsgFileMgr::save(Type type, const QString& filename, const ToolsMessagesList& msgs)
{
    QString filenameTmp(filename);
    while (true) {
        filenameTmp.append(".tmp");
        if (!QFile::exists(filenameTmp)) {
            break;
        }
    }

    QFile msgsFile(filenameTmp);
    if (!msgsFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    auto convertedList = convertMsgList(type, msgs);

    auto jsonArray = QJsonArray::fromVariantList(convertedList);
    QJsonDocument jsonDoc(jsonArray);
    auto data = jsonDoc.toJson();

    msgsFile.write(data);

    if ((QFile::exists(filename)) &&
        (!QFile::remove(filename))) {
        msgsFile.close();
        QFile::remove(filenameTmp);
        return false;
    }

    if (!msgsFile.rename(filename)) {
        return false;
    }

    m_lastFile = filename;
    return true;
}

const QString& ToolsMsgFileMgr::getFilesFilter()
{
    static const QString Str(QObject::tr("All Files (*)"));
    return Str;
}

ToolsMsgFileMgr::FileSaveHandler ToolsMsgFileMgr::startRecvSave(const QString& filename)
{
    auto handler = std::unique_ptr<QFile>(new QFile(filename));
    if (!handler->open(QIODevice::WriteOnly)) {
        handler.reset();
        return FileSaveHandler();
    }

    handler->write("[\n");
    return
        FileSaveHandler(
            handler.release(),
            [](QFile* ptr)
            {
                ptr->write("\n]\n");
                delete ptr;
            });
}

void ToolsMsgFileMgr::addToRecvSave(
    FileSaveHandler handler,
    const ToolsMessage& msg,
    bool flush)
{
    assert(handler);
    auto msgMap = convertRecvMsg(msg);
    if (!msgMap.isEmpty()) {
        auto jsonObj = QJsonObject::fromVariantMap(msgMap);
        QJsonDocument jsonDoc(jsonObj);
        auto data = jsonDoc.toJson();
        assert(!data.isEmpty());
        if (data[data.size() - 1] == '\n') {
            data.resize(data.size() - 1);
        }

        static const char* IndicatorPropName = "first_write_performed";
        auto indicatorVar = handler->property(IndicatorPropName);
        bool firstWritePerformed = indicatorVar.isValid();

        if (firstWritePerformed) {
            handler->write(",\n");
        }
        else {
            handler->setProperty(IndicatorPropName, true);
        }

        handler->write(data);
    }

    if (flush) {
        handler->flush();
    }
}

void ToolsMsgFileMgr::flushRecvFile(FileSaveHandler handler)
{
    assert(handler);
    handler->flush();
}
}  // namespace cc_tools_qt


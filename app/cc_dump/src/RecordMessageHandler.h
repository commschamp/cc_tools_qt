//
// Copyright 2016 - 2021 (C). Alex Robenko. All rights reserved.
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

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtCore/QString>
CC_ENABLE_WARNINGS()

#include "cc_tools_qt/MessageHandler.h"
#include "cc_tools_qt/MsgFileMgr.h"

namespace comms_dump
{

class RecordMessageHandler : public cc_tools_qt::MessageHandler
{
public:
    RecordMessageHandler(const QString& filename);

    virtual ~RecordMessageHandler() noexcept;

    void flush();

protected:
    virtual void beginMsgHandlingImpl(cc_tools_qt::Message& msg) override;

private:
    typedef cc_tools_qt::MsgFileMgr::FileSaveHandler FileSaveHandler;
    FileSaveHandler m_saveHandler;

};

}  // namespace comms_dump



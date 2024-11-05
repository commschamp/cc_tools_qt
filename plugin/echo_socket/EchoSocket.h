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


#pragma once

#include "cc_tools_qt/ToolsSocket.h"

#include <QtCore/QTimer>

#include <list>

namespace cc_tools_qt
{

namespace plugin
{

class EchoSocket : public QObject, public cc_tools_qt::ToolsSocket
{
    Q_OBJECT

public:
    EchoSocket();
    ~EchoSocket() noexcept;

protected:
    virtual void sendDataImpl(DataInfoPtr dataPtr) override;
    virtual unsigned connectionPropertiesImpl() const override;

private slots:
    void sendDataPostponed();

private:
    QTimer m_timer;
    std::list<DataInfoPtr> m_pendingData;
    bool m_timerActive = false;
};

inline
ToolsSocketPtr makeEchoSocket()
{
    return ToolsSocketPtr(new EchoSocket());
}

} // namespace plugin

}  // namespace cc_tools_qt



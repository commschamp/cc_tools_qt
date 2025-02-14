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

#include "EchoSocket.h"

#include <cassert>

namespace cc_tools_qt
{

namespace plugin
{

EchoSocket::EchoSocket()
{
    m_timer.setSingleShot(true);
    connect(
        &m_timer, &QTimer::timeout,
        this, &EchoSocket::sendDataPostponed,
        Qt::QueuedConnection);
}

EchoSocket::~EchoSocket() noexcept = default;

void EchoSocket::sendDataImpl(ToolsDataInfoPtr dataPtr)
{
    m_pendingData.push_back(std::move(dataPtr));
    if (m_timerActive) {
        return;
    }

    m_timerActive = true;
    m_timer.start(0);
}

unsigned EchoSocket::connectionPropertiesImpl() const
{
    return ConnectionProperty_Autoconnect | ConnectionProperty_NonDisconnectable;
}

void EchoSocket::sendDataPostponed()
{
    m_timerActive = false;
    while (!m_pendingData.empty()) {
        auto dataPtr = m_pendingData.front();
        m_pendingData.pop_front();

        auto inDataPtr = makeDataInfo();
        inDataPtr->m_data = dataPtr->m_data;
        inDataPtr->m_extraProperties = dataPtr->m_extraProperties;
        inDataPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();
        reportDataReceived(std::move(inDataPtr));
    }
}

} // namespace plugin

}  // namespace cc_tools_qt



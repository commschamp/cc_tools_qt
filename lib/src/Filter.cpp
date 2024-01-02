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

#include "cc_tools_qt/Filter.h"

namespace cc_tools_qt
{

Filter::Filter() = default;
Filter::~Filter() noexcept = default;

bool Filter::start()
{
    return startImpl();
}

void Filter::stop()
{
    stopImpl();
}

QList<DataInfoPtr> Filter::recvData(DataInfoPtr dataPtr)
{
    return recvDataImpl(std::move(dataPtr));
}

QList<DataInfoPtr> Filter::sendData(DataInfoPtr dataPtr)
{
    return sendDataImpl(std::move(dataPtr));
}

void Filter::socketConnectionReport(bool connected)
{
    return socketConnectionReportImpl(connected);
}

bool Filter::startImpl()
{
    return true;
}

void Filter::stopImpl()
{
}

void Filter::socketConnectionReportImpl(bool connected)
{
    static_cast<void>(connected);
}

void Filter::reportDataToSend(DataInfoPtr dataPtr)
{
    if (m_dataToSendCallback) {
        m_dataToSendCallback(std::move(dataPtr));
    }
}

void Filter::reportError(const QString& msg)
{
    if (m_errorReportCallback) {
        m_errorReportCallback(msg);
    }
}

}  // namespace cc_tools_qt


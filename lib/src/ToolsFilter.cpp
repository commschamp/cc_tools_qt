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

#include "cc_tools_qt/ToolsFilter.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace cc_tools_qt
{

namespace 
{

std::string dataToStr(const DataInfo::DataSeq& data)
{
    std::stringstream stream;
    stream << std::hex;

    for (auto byte : data) {
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte) << ' ';
    }

    return stream.str();
}

} // namespace 
    

ToolsFilter::ToolsFilter() = default;
ToolsFilter::~ToolsFilter() noexcept = default;

bool ToolsFilter::start()
{
    return startImpl();
}

void ToolsFilter::stop()
{
    stopImpl();
}

QList<DataInfoPtr> ToolsFilter::recvData(DataInfoPtr dataPtr)
{
    unsigned long long milliseconds = 0U;

    if (1U <= m_debugLevel) {
        auto timestamp = dataPtr->m_timestamp;
        if (timestamp == DataInfo::Timestamp()) {
            timestamp = DataInfo::TimestampClock::now();
        }

        auto sinceEpoch = timestamp.time_since_epoch();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] (" << debugNameImpl() << ") <-- " << dataPtr->m_data.size() << " bytes"; 
        if (2U <= m_debugLevel) {
            std::cout << " | " << dataToStr(dataPtr->m_data);
        }
        std::cout << std::endl;
    }

    auto result = recvDataImpl(std::move(dataPtr));
    if (1U <= m_debugLevel) {
        for (auto& resultDataPtr : result) {
            std::cout << '[' << milliseconds << "] " << resultDataPtr->m_data.size() << " bytes <-- (" << debugNameImpl() << ")"; 
            if (2U <= m_debugLevel) {
                std::cout << " | " << dataToStr(resultDataPtr->m_data);
            }
            std::cout << std::endl;
        }
    }    
    return result;
}

QList<DataInfoPtr> ToolsFilter::sendData(DataInfoPtr dataPtr)
{
    unsigned long long milliseconds = 0U;

    if (0U < m_debugLevel) {
        auto timestamp = dataPtr->m_timestamp;
        if (timestamp == DataInfo::Timestamp()) {
            timestamp = DataInfo::TimestampClock::now();
        }

        auto sinceEpoch = timestamp.time_since_epoch();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] " << dataPtr->m_data.size() << " bytes --> (" << debugNameImpl() << ")"; 
        if (1U < m_debugLevel) {
            std::cout << " | " << dataToStr(dataPtr->m_data);
        }
        std::cout << std::endl;
    }

    auto result = sendDataImpl(std::move(dataPtr));
    if (0U < m_debugLevel) {
        for (auto& resultDataPtr : result) {
            std::cout << '[' << milliseconds << "] (" << debugNameImpl() << ") --> " << resultDataPtr->m_data.size() << " bytes"; 
            if (1U < m_debugLevel) {
                std::cout << " | " << dataToStr(resultDataPtr->m_data);
            }
            std::cout << std::endl;
        }
    }
    return result;
}

void ToolsFilter::socketConnectionReport(bool connected)
{
    return socketConnectionReportImpl(connected);
}

void ToolsFilter::applyInterPluginConfig(const QVariantMap& props)
{
    applyInterPluginConfigImpl(props);
}

void ToolsFilter::setDebugOutputLevel(unsigned level)
{
    m_debugLevel = level;
}

bool ToolsFilter::startImpl()
{
    return true;
}

void ToolsFilter::stopImpl()
{
}

QList<DataInfoPtr> ToolsFilter::recvDataImpl(DataInfoPtr dataPtr)
{
    QList<DataInfoPtr> result;
    result.append(std::move(dataPtr));
    return result;
}

QList<DataInfoPtr> ToolsFilter::sendDataImpl(DataInfoPtr dataPtr)
{
    QList<DataInfoPtr> result;
    result.append(std::move(dataPtr));
    return result;
}

void ToolsFilter::socketConnectionReportImpl([[maybe_unused]] bool connected)
{
}

void ToolsFilter::applyInterPluginConfigImpl([[maybe_unused]] const QVariantMap& props)
{
}

const char* ToolsFilter::debugNameImpl() const
{
    return "filter";
}

void ToolsFilter::reportDataToSend(DataInfoPtr dataPtr)
{
    if (0U < m_debugLevel) {
        auto timestamp = dataPtr->m_timestamp;
        if (timestamp == DataInfo::Timestamp()) {
            timestamp = DataInfo::TimestampClock::now();
        }

        auto sinceEpoch = timestamp.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] (" << debugNameImpl() << ") --> " << dataPtr->m_data.size() << " bytes"; 
        if (1U < m_debugLevel) {
            std::cout << " | " << dataToStr(dataPtr->m_data);
        }
        std::cout << std::endl;
    }

    if (m_dataToSendCallback) {
        m_dataToSendCallback(std::move(dataPtr));
    }
}

void ToolsFilter::reportError(const QString& msg)
{
    if (m_errorReportCallback) {
        m_errorReportCallback(msg);
    }
}

void ToolsFilter::reportInterPluginConfig(const QVariantMap& props)
{
    if (m_interPluginConfigReportCallback) {
        m_interPluginConfigReportCallback(props);
    }
}

unsigned long long ToolsFilter::currTimestamp()
{
    auto timestamp = std::chrono::high_resolution_clock::now();
    auto sinceEpoch = timestamp.time_since_epoch();
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
    return milliseconds;
}

unsigned ToolsFilter::getDebugOutputLevel() const
{
    return m_debugLevel;
}

}  // namespace cc_tools_qt


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

#include "cc_tools_qt/ToolsFrame.h"

namespace demo
{

namespace cc_plugin
{

class DemoFrameImpl;
class DemoFrame : public  cc_tools_qt::ToolsFrame
{
public:
    DemoFrame();
    virtual ~DemoFrame();

protected:
    virtual cc_tools_qt::ToolsMessagesList readDataImpl(const cc_tools_qt::ToolsDataInfo& dataInfo, bool final) override;
    virtual void updateMessageImpl(cc_tools_qt::ToolsMessage& msg) override;
    virtual cc_tools_qt::ToolsMessagePtr createInvalidMessageImpl() override;
    virtual cc_tools_qt::ToolsMessagePtr createRawDataMessageImpl() override;
    virtual cc_tools_qt::ToolsMessagePtr createExtraInfoMessageImpl() override;
    virtual cc_tools_qt::ToolsMessagesList createAllMessagesImpl() override;
    virtual cc_tools_qt::ToolsMessagePtr createMessageImpl(const QString& idAsString, unsigned idx) override;
    virtual DataSeq writeProtMsgImpl(const void* protInterface) override;

private:
    std::unique_ptr<DemoFrameImpl> m_pImpl;
};

}  // namespace cc_plugin

}  // namespace demo


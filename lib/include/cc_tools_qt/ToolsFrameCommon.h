//
// Copyright 2024 - 2025 (C). Alex Robenko. All rights reserved.
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

namespace cc_tools_qt
{

template <typename TMsgBase>
class ToolsFrameCommon : public ToolsFrame
{
public:
    using ProtInterface = typename TMsgBase::ProtInterface;
    using DataSeq = typename TMsgBase::DataSeq;

    DataSeq writeProtMsg(const ProtInterface& msg)
    {
        return writeProtMsgImpl(msg);
    } 

protected:
    ToolsFrameCommon() = default;

    virtual DataSeq writeProtMsgImpl(const ProtInterface& msg) = 0;
};

}  // namespace cc_tools_qt



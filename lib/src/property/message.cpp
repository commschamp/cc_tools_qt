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

#include "cc_tools_qt/property/message.h"

namespace cc_tools_qt
{

namespace property
{

namespace message
{

ToolsMsgType::ToolsMsgType() : Base("cc.msg_type") {}
ToolsMsgIdx::ToolsMsgIdx() : Base("cc.msg_idx") {}
ToolsMsgTimestamp::ToolsMsgTimestamp() : Base("cc.msg_timestamp") {}
ToolsMsgProtocolName::ToolsMsgProtocolName() : Base("cc.msg_prot_name") {}
ToolsMsgTransportMsg::ToolsMsgTransportMsg() : Base("cc.msg_transport") {}
ToolsMsgRawDataMsg::ToolsMsgRawDataMsg() : Base("cc.msg_raw_data") {} 
ToolsMsgExtraInfoMsg::ToolsMsgExtraInfoMsg() : Base("cc.msg_extra_info") {} 
ToolsMsgExtraInfo::ToolsMsgExtraInfo() : Base("cc.msg_extra_info_map") {} 
ToolsMsgForceExtraInfoExistence::ToolsMsgForceExtraInfoExistence() : Base("cc.force_extra_info_exist") {} 
ToolsMsgDelay::ToolsMsgDelay() : Base("cc.msg_delay") {} 
ToolsMsgDelayUnits::ToolsMsgDelayUnits() : Base("cc.msg_delay_units") {} 
ToolsMsgRepeatDuration::ToolsMsgRepeatDuration() : Base("cc.msg_repeat") {} 
ToolsMsgRepeatDurationUnits::ToolsMsgRepeatDurationUnits() : Base("cc.msg_repeat_units") {} 
ToolsMsgRepeatCount::ToolsMsgRepeatCount() : Base("cc.msg_repeat_count") {} 
ToolsMsgScrollPos::ToolsMsgScrollPos() : Base("cc.msg_scroll_pos") {} 
ToolsMsgComment::ToolsMsgComment() : Base("cc.msg_comment") {} 

}  // namespace message

}  // namespace property

}  // namespace cc_tools_qt


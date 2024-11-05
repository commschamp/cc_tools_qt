
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

#include "cc_tools_qt/ToolsDataInfo.h"

namespace cc_tools_qt
{

CC_TOOLS_API ToolsDataInfoPtr makeDataInfo()
{
    return ToolsDataInfoPtr(new ToolsDataInfo());
}

CC_TOOLS_API ToolsDataInfoPtr makeDataInfoTimed()
{
    auto info = makeDataInfo();
    info->m_timestamp = ToolsDataInfo::TimestampClock::now();
    return info;
}

} // namespace cc_tools_qt

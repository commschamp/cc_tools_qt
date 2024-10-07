//
// Copyright 2015 - 2018 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsMessageBase.h"
#include "cc_tools_qt/ToolsMessageInterface.h"
#include "demo/message/FloatValues.h"
#include "demo/DemoMessage.h"
#include "DemoMessage.h"

namespace demo
{

namespace cc_plugin
{

namespace message
{

class FloatValues : public
    cc_tools_qt::ToolsMessageBase<
        demo::message::FloatValues<cc_tools_qt::ToolsMessageInterface<demo::DemoMessage>>,
        demo::cc_plugin::message::FloatValues,
        demo::cc_plugin::DemoMessage>
{
public:
    FloatValues();
    FloatValues(const FloatValues&) = default;
    FloatValues(FloatValues&&) = default;
    virtual ~FloatValues() noexcept;

    FloatValues& operator=(const FloatValues&);
    FloatValues& operator=(FloatValues&&);

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


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
#include "cc_tools_qt/ToolsProtMsgInterface.h"
#include "demo/message/IntValues.h"
#include "demo/DemoMessage.h"
#include "DemoMessage.h"

namespace demo
{

namespace cc_plugin
{

namespace message
{

class IntValues : public 
    cc_tools_qt::ToolsMessageBase<
        demo::message::IntValues<cc_tools_qt::ToolsProtMsgInterface<demo::DemoMessage>>,
        demo::cc_plugin::message::IntValues,
        demo::cc_plugin::DemoMessage>
{
public:
    IntValues();
    IntValues(const IntValues&) = default;
    IntValues(IntValues&&) = default;
    virtual ~IntValues() noexcept;

    IntValues& operator=(const IntValues&);
    IntValues& operator=(IntValues&&);

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


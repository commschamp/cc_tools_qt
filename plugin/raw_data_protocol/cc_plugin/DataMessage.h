//
// Copyright 2016 - 2023 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/cc_tools_qt.h"
#include "raw_data_protocol/DataMessage.h"
#include "cc_plugin/Message.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace raw_data_protocol
{

namespace cc_plugin
{

class DataMessage : public
    cc_tools_qt::ProtocolMessageBase<
        raw_data_protocol::DataMessage<raw_data_protocol::cc_plugin::Message>,
        DataMessage>
{
public:
    DataMessage();
    DataMessage(const DataMessage&) = delete;
    DataMessage(DataMessage&&) = delete;
    virtual ~DataMessage() noexcept;

    DataMessage& operator=(const DataMessage&);
    DataMessage& operator=(DataMessage&&);

protected:
    virtual const char* nameImpl() const override;
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace cc_plugin

}  // namespace raw_data_protocol

}  // namespace plugin

}  // namespace cc_tools_qt


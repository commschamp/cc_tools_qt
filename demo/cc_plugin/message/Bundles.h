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

#include "cc_tools_qt/cc_tools_qt.h"
#include "demo/message/Bundles.h"
#include "cc_plugin/DemoMessage.h"

namespace demo
{

namespace cc_plugin
{

namespace message
{

class Bundles : public
    cc_tools_qt::ProtocolMessageBase<
        demo::message::Bundles<demo::cc_plugin::DemoMessage>,
        Bundles>
{
public:
    Bundles();
    Bundles(const Bundles&) = delete;
    Bundles(Bundles&&) = delete;
    virtual ~Bundles() noexcept;

    Bundles& operator=(const Bundles&);
    Bundles& operator=(Bundles&&);

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


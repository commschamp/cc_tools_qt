//
// Copyright 2017 - 2018 (C). Alex Robenko. All rights reserved.
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
#include "demo/message/Variants.h"
#include "cc_plugin/Message.h"

namespace demo
{

namespace cc_plugin
{

namespace message
{

class Variants : public
    cc_tools_qt::ProtocolMessageBase<
        demo::message::Variants<demo::cc_plugin::Message>,
        Variants>
{
public:
    Variants();
    Variants(const Variants&) = delete;
    Variants(Variants&&) = delete;
    virtual ~Variants() noexcept;

    Variants& operator=(const Variants&);
    Variants& operator=(Variants&&);

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


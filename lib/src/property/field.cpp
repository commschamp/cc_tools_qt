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

#include "cc_tools_qt/property/field.h"

namespace cc_tools_qt
{

namespace property
{

namespace field
{

Common::Common() = default;
Common::Common(const Common&) = default;
Common::Common(Common&&) = default;

Common::Common(const QVariantMap& props)
{
    getFrom(props);
}

Common::Common(const QVariant& props)
{
    if (props.isValid() && props.canConvert<QVariantMap>()) {
        getFrom(props.value<QVariantMap>());
    }
}

Common::~Common() noexcept = default;

Common& Common::operator=(const Common&) = default;
Common& Common::operator=(Common&&) = default;

void Common::setTo([[maybe_unused]] QVariantMap& props) const
{
}

void Common::getFrom([[maybe_unused]] const QVariantMap& props)
{
}

QVariantMap Common::asMap() const
{
    QVariantMap props;
    setTo(props);
    return props;
}


}  // namespace field

}  // namespace property

}  // namespace cc_tools_qt



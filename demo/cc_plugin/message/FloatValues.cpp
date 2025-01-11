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

#include "FloatValues.h"

#include <cassert>

namespace demo
{

namespace cc_plugin
{

namespace message
{

FloatValues::FloatValues() = default;
FloatValues::~FloatValues() noexcept = default;

FloatValues& FloatValues::operator=(const FloatValues&) = default;
FloatValues& FloatValues::operator=(FloatValues&&) = default;

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo


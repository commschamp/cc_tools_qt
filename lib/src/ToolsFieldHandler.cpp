//
// Copyright 2016 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsFieldHandler.h"

namespace cc_tools_qt
{

void ToolsFieldHandler::handle(field::ToolsArrayListField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsBitfieldField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsBitmaskField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsBundleField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsEnumField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsFloatField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsIntField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsOptionalField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsRawDataField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsStringField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsUnknownField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsUnsignedLongField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle(field::ToolsVariantField& field)
{
    handle(static_cast<ToolsField&>(field));
}

void ToolsFieldHandler::handle([[maybe_unused]] ToolsField& field)
{
}

}  // namespace cc_tools_qt


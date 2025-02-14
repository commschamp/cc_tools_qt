//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#include <QtWidgets/QWidget>

#include "cc_tools_qt/ToolsMessage.h"

namespace cc_tools_qt
{

class MessageDisplayWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    MessageDisplayWidget(QWidget* parentObj = nullptr)
      : Base(parentObj)
    {
    }

public slots:

    void displayMessage(ToolsMessagePtr msg, bool force = false)
    {
        displayMessageImpl(msg, force);
    }

    void setEditEnabled(bool enabled)
    {
        setEditEnabledImpl(enabled);
    }

    void clear()
    {
        clearImpl();
    }

    void refresh()
    {
        refreshImpl();
    }

signals:
    void sigMsgUpdated();

protected:
    virtual void displayMessageImpl(ToolsMessagePtr msg, bool force) = 0;
    virtual void setEditEnabledImpl(bool enabled) = 0;
    virtual void clearImpl() = 0;
    virtual void refreshImpl() = 0;
};

}  // namespace cc_tools_qt



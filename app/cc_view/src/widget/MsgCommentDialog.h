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

#include "ui_MsgCommentDialog.h"

#include "cc_tools_qt/ToolsMessage.h"

#include <QtWidgets/QDialog>

namespace cc_tools_qt
{

class MsgCommentDialog : public QDialog
{
    Q_OBJECT
    using Base = QDialog;
public:
    MsgCommentDialog(
        ToolsMessagePtr msg,
        QWidget* parentObj = nullptr);

private slots:
    void accept();

private:
    Ui::MsgCommentDialog m_ui;
    ToolsMessagePtr m_msg;
};

}  // namespace cc_tools_qt


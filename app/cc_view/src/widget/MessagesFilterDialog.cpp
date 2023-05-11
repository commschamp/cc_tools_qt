//
// Copyright 2023 - 2023 (C). Alex Robenko. All rights reserved.
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


#include "MessagesFilterDialog.h"

#include <QtWidgets/QScrollBar>

namespace cc_tools_qt
{

namespace 
{


} // namespace 
    

MessagesFilterDialog::MessagesFilterDialog(
    FilteredMessages& filteredMessages,
    ProtocolPtr protocol,
    QWidget* parentObj)
  : Base(parentObj),
    m_filteredMessages(filteredMessages),
    m_hiddenMessages(m_filteredMessages)
{
    m_ui.setupUi(this);

    auto allMessages = protocol->createAllMessages();
    for (auto& msgPtr : allMessages) {
        m_visibleMessages.push_back(GuiAppMgr::messageDesc(*msgPtr));
    }

    std::sort(m_visibleMessages.begin(), m_visibleMessages.end());
    std::sort(m_hiddenMessages.begin(), m_hiddenMessages.end());

    m_visibleMessages.erase(
        std::remove_if(
            m_visibleMessages.begin(), m_visibleMessages.end(),
            [this](const QString& val)
            {
                auto iter = std::lower_bound(m_hiddenMessages.begin(), m_hiddenMessages.end(), val);
                return (iter != m_hiddenMessages.end()) && (*iter == val);
            }),
        m_visibleMessages.end()
    );

    refresh();

    assert(parentObj);
    auto newHeight = std::max(height(), (parentObj->height() * 9) / 10);
    auto newWidth = std::max(width(), (parentObj->width() * 7) / 10);
    resize(QSize(newWidth, newHeight));

    connect(
        m_ui.m_hideAllPushButton, SIGNAL(clicked()),
        this, SLOT(hideAllClicked()));

    connect(
        m_ui.m_hideSelectedPushButton, SIGNAL(clicked()),
        this, SLOT(hideSelectedClicked()));  

    connect(
        m_ui.m_showSelectedPushButton, SIGNAL(clicked()),
        this, SLOT(showSelectedClicked()));                

    connect(
        m_ui.m_showAllPushButton, SIGNAL(clicked()),
        this, SLOT(showAllClicked()));        

    connect(
        m_ui.m_visibleListWidget, SIGNAL(itemSelectionChanged()),
        this, SLOT(visibleSelected()));

    connect(
        m_ui.m_visibleListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(hideVisible(QListWidgetItem*)));        

    connect(
        m_ui.m_hiddenListWidget, SIGNAL(itemSelectionChanged()),
        this, SLOT(hiddenSelected()));   

    connect(
        m_ui.m_clearSearchToolButton, SIGNAL(clicked()),
        this, SLOT(clearSearchText()));              

    connect(
        m_ui.m_hiddenListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(showHidden(QListWidgetItem*)));            

    connect(
        m_ui.m_searchLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(refreshDisplayedLists(const QString&)));
}

void MessagesFilterDialog::refreshDisplayedLists(const QString& searchText)
{
    m_searchText = searchText;
    refresh();
}

void MessagesFilterDialog::accept()
{
    m_filteredMessages = m_hiddenMessages;
    assert(std::is_sorted(m_filteredMessages.begin(), m_filteredMessages.end()));
    Base::accept();
}

void MessagesFilterDialog::hideAllClicked()
{
    moveAllMessages(m_visibleMessages, m_hiddenMessages);
}

void MessagesFilterDialog::hideSelectedClicked()
{
    moveSelectedMessages(*m_ui.m_visibleListWidget, m_visibleMessages, m_hiddenMessages);
}

void MessagesFilterDialog::showSelectedClicked()
{
    moveSelectedMessages(*m_ui.m_hiddenListWidget, m_hiddenMessages, m_visibleMessages);
}

void MessagesFilterDialog::showAllClicked()
{
    moveAllMessages(m_hiddenMessages, m_visibleMessages);
}

void MessagesFilterDialog::clearSearchText()
{
    m_ui.m_searchLineEdit->clear();
}

void MessagesFilterDialog::visibleSelected()
{
    auto selected = m_ui.m_visibleListWidget->selectedItems();
    if (selected.isEmpty()) {
        return;
    }

    m_ui.m_hiddenListWidget->clearSelection();
    refreshButtons();
}

void MessagesFilterDialog::hiddenSelected()
{
    auto selected = m_ui.m_hiddenListWidget->selectedItems();
    if (selected.isEmpty()) {
        return;
    }

    m_ui.m_visibleListWidget->clearSelection();
    refreshButtons();
}

void MessagesFilterDialog::hideVisible(QListWidgetItem* item)
{
    moveSelectedItem(item, m_visibleMessages, m_hiddenMessages);
}

void MessagesFilterDialog::showHidden(QListWidgetItem* item)
{
    moveSelectedItem(item, m_hiddenMessages, m_visibleMessages);
}

void MessagesFilterDialog::refreshButtons()
{
    m_ui.m_hideAllPushButton->setEnabled(!m_visibleMessages.empty());
    m_ui.m_showAllPushButton->setEnabled(!m_hiddenMessages.empty());
    m_ui.m_hideSelectedPushButton->setEnabled(!m_ui.m_visibleListWidget->selectedItems().empty());
    m_ui.m_showSelectedPushButton->setEnabled(!m_ui.m_hiddenListWidget->selectedItems().empty());
}

void MessagesFilterDialog::refreshMessagesList(const MessagesFilterDialog::FilteredMessages& messages, QListWidget& list, QScrollArea& scroll)
{
    auto* scrollBar = scroll.verticalScrollBar();
    auto scrollValue = 0;
    if (scrollBar != nullptr) {
        scrollValue = scrollBar->value();
    }

    list.clear();
    for (auto& m : messages) {
        if ((!m_searchText.isEmpty()) && (!m.contains(m_searchText, Qt::CaseInsensitive))) {
            continue;
        }

        list.addItem(m);
    }

    auto* updatedScrollBar = scroll.verticalScrollBar();
    if (updatedScrollBar != nullptr) {
        updatedScrollBar->setValue(scrollValue);
    }
}

void MessagesFilterDialog::refreshMessages()
{
    refreshMessagesList(m_visibleMessages, *m_ui.m_visibleListWidget, *m_ui.m_visibleScrollArea);
    refreshMessagesList(m_hiddenMessages, *m_ui.m_hiddenListWidget, *m_ui.m_hiddenScrollArea);
}

void MessagesFilterDialog::refresh()
{
    refreshMessages();
    refreshButtons();
}

void MessagesFilterDialog::moveAllMessages(FilteredMessages& from, FilteredMessages& to)
{
    std::move(from.begin(), from.end(), std::back_inserter(to));
    std::sort(to.begin(), to.end());
    from.clear();
    refresh();    
}

void MessagesFilterDialog::moveSelectedMessages(QListWidget& list, FilteredMessages& from, FilteredMessages& to)
{
    auto selected = list.selectedItems();
    for (auto* item : selected) {
        moveSelectedItem(item, from, to, false);
    }

    std::sort(to.begin(), to.end());
    refresh();
}

void MessagesFilterDialog::moveSelectedItem(const QListWidgetItem* item, FilteredMessages& from, FilteredMessages& to, bool finalUpdate)
{
    auto iter = std::find(from.begin(), from.end(), item->text());
    if (iter == from.end()) {
        assert(false); // Should not happen
        return;
    }

    to.push_back(std::move(*iter));
    from.erase(iter);

    if (finalUpdate) {
        std::sort(to.begin(), to.end());
        refresh();        
    }
}

}  // namespace cc_tools_qt



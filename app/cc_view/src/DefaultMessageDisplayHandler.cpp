//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "DefaultMessageDisplayHandler.h"

#include <cassert>

#include <QtWidgets/QApplication>

#include "cc_tools_qt/ToolsField.h"
#include "cc_tools_qt/ToolsFieldHandler.h"
#include "widget/field/IntValueFieldWidget.h"
#include "widget/field/UnsignedLongLongIntValueFieldWidget.h"
#include "widget/field/BitmaskValueFieldWidget.h"
#include "widget/field/EnumValueFieldWidget.h"
#include "widget/field/StringFieldWidget.h"
#include "widget/field/BitfieldFieldWidget.h"
#include "widget/field/OptionalFieldWidget.h"
#include "widget/field/BundleFieldWidget.h"
#include "widget/field/ArrayListRawDataFieldWidget.h"
#include "widget/field/ArrayListFieldWidget.h"
#include "widget/field/FloatValueFieldWidget.h"
#include "widget/field/VariantFieldWidget.h"
#include "widget/field/UnknownValueFieldWidget.h"

namespace cc_tools_qt
{

namespace
{

class WidgetCreator : public ToolsFieldHandler
{
public:

    WidgetCreator()
    {
        auto widgets = qApp->topLevelWidgets();
        if (!widgets.isEmpty()) {
            m_parent = widgets[0];
        }
    }

    virtual void handle(field::ToolsIntField& field) override
    {
        m_widget.reset(new IntValueFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsUnsignedLongField& field) override
    {
        m_widget.reset(new UnsignedLongLongIntValueFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsBitmaskField& field) override
    {
        m_widget.reset(new BitmaskValueFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsEnumField& field) override
    {
        m_widget.reset(new EnumValueFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsStringField& field) override
    {
        m_widget.reset(new StringFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsBitfieldField& field) override
    {
        auto& membersWrappers = field.getMembers();
        std::vector<FieldWidgetPtr> membersWidgets;
        membersWidgets.reserve(membersWrappers.size());
        for (auto& mem : membersWrappers) {
            mem->dispatch(*this);
            membersWidgets.push_back(getWidget());
        }

        std::unique_ptr<BitfieldFieldWidget> widget(new BitfieldFieldWidget(field.actClone(), m_parent));
        for (auto& memWidget : membersWidgets) {
            widget->addMemberField(memWidget.release());
        }

        m_widget = std::move(widget);
    }

    virtual void handle(field::ToolsOptionalField& field) override
    {
        field.getFieldWrapper().dispatch(*this);
        auto wrappedWidget = getWidget();

        std::unique_ptr<OptionalFieldWidget> widget(new OptionalFieldWidget(field.actClone(), m_parent));
        widget->setField(wrappedWidget.release());
        m_widget = std::move(widget);
    }

    virtual void handle(field::ToolsBundleField& field) override
    {
        auto& memberFields = field.getMembers();
        std::vector<FieldWidgetPtr> membersWidgets;
        membersWidgets.reserve(memberFields.size());
        for (auto& mem : memberFields) {
            mem->dispatch(*this);
            membersWidgets.push_back(getWidget());
        }

        std::unique_ptr<BundleFieldWidget> widget(new BundleFieldWidget(field.actClone(), m_parent));
        for (auto& memWidget : membersWidgets) {
            widget->addMemberField(memWidget.release());
        }

        m_widget = std::move(widget);
    }

    virtual void handle(field::ToolsRawDataField& field) override
    {
        m_widget.reset(new ArrayListRawDataFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsArrayListField& field) override
    {
        auto createMembersWidgetsFunc =
            [](field::ToolsArrayListField& fieldParam) -> std::vector<FieldWidgetPtr>
            {
                std::vector<FieldWidgetPtr> allFieldsWidgets;
                WidgetCreator otherCreator;
                auto& memWrappers = fieldParam.getMembers();
                allFieldsWidgets.reserve(memWrappers.size());
                assert(memWrappers.size() == fieldParam.size());

                for (auto& memWrap : memWrappers) {
                    memWrap->dispatch(otherCreator);
                    allFieldsWidgets.push_back(otherCreator.getWidget());
                }

                assert(allFieldsWidgets.size() == fieldParam.size());
                return allFieldsWidgets;
            };

        assert(field.size() == field.getMembers().size());
        m_widget.reset(new ArrayListFieldWidget(field.actClone(), std::move(createMembersWidgetsFunc), m_parent));
    }

    virtual void handle(field::ToolsFloatField& field) override
    {
        m_widget.reset(new FloatValueFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle(field::ToolsVariantField& field) override
    {
        auto createMemberWidgetsFunc =
            [](ToolsField& wrap) -> FieldWidgetPtr
            {
                WidgetCreator otherCreator;
                wrap.dispatch(otherCreator);
                return otherCreator.getWidget();
            };

        FieldWidgetPtr memberWidget;
        auto* memberFieldPtr = field.getCurrent();
        if (memberFieldPtr != nullptr) {
            memberFieldPtr->dispatch(*this);
            memberWidget = getWidget();
        }

        std::unique_ptr<VariantFieldWidget> widget(
                    new VariantFieldWidget(
                        field.actClone(),
                        createMemberWidgetsFunc,
                        m_parent));
        if (memberWidget) {
            widget->setMemberField(memberWidget.release());
        }

        m_widget = std::move(widget);
    }

    virtual void handle(field::ToolsUnknownField& field) override
    {
        m_widget.reset(new UnknownValueFieldWidget(field.actClone(), m_parent));
    }

    virtual void handle([[maybe_unused]] ToolsField& field) override
    {
        [[maybe_unused]] static constexpr bool Unexpected_field = false;
        assert(Unexpected_field);
    }

    FieldWidgetPtr getWidget()
    {
        return std::move(m_widget);
    }


private:
    FieldWidgetPtr m_widget;
    QWidget* m_parent = nullptr;
};



}  // namespace

DefaultMessageDisplayHandler::~DefaultMessageDisplayHandler() noexcept = default;

DefaultMessageDisplayHandler::MsgWidgetPtr DefaultMessageDisplayHandler::getMsgWidget(Message& msg)
{
    auto widget = std::make_unique<DefaultMessageWidget>(msg);

    auto transportFields = msg.transportFields();
    for (auto& f : transportFields) {
        WidgetCreator creator;
        f->dispatch(creator);
        auto fieldWidget = creator.getWidget();
        fieldWidget->hide();
        widget->addExtraTransportFieldWidget(fieldWidget.release());        
    }

    auto fields = msg.payloadFields();
    for (auto& f : fields) {
        WidgetCreator creator;
        f->dispatch(creator);
        auto fieldWidget = creator.getWidget();
        fieldWidget->hide();
        widget->addFieldWidget(fieldWidget.release());        
    }

    return widget;
}

}  // namespace cc_tools_qt



function (bin_cc_view)
    set (name "cc_view")
    
    set (src
        main.cpp
        dir.cpp
        GuiAppMgr.cpp
        DefaultMessageDisplayHandler.cpp
        MsgFileMgrG.cpp
        PluginMgrG.cpp
        MsgMgrG.cpp
        ShortcutMgr.cpp
        icon.cpp
        widget/MainWindowWidget.cpp
        widget/MainToolbar.cpp
        widget/LeftPaneWidget.cpp
        widget/RightPaneWidget.cpp
        widget/DefaultMessageDisplayWidget.cpp
        widget/RecvAreaToolBar.cpp
        widget/SendAreaToolBar.cpp
        widget/MsgListWidget.cpp
        widget/RecvMsgListWidget.cpp
        widget/SendMsgListWidget.cpp
        widget/MessageWidget.cpp
        widget/MsgDetailsWidget.cpp
        widget/RawHexDataDialog.cpp
        widget/DefaultMessageWidget.cpp
        widget/ProtocolsStackWidget.cpp
        widget/MessageUpdateDialog.cpp
        widget/PluginsListWidget.cpp
        widget/PluginConfigDialog.cpp
        widget/PluginConfigWrapWidget.cpp
        widget/PluginConfigWrapsListWidget.cpp
        widget/MsgCommentDialog.cpp
        widget/MessagesFilterDialog.cpp
        widget/MessageDisplayWidget.h
        widget/field/FieldWidget.cpp
        widget/field/ShortIntValueFieldWidget.cpp
        widget/field/LongIntValueFieldWidget.cpp
        widget/field/LongLongIntValueFieldWidget.cpp
        widget/field/UnsignedLongLongIntValueFieldWidget.cpp
        widget/field/ScaledIntValueFieldWidget.cpp
        widget/field/IntValueFieldWidget.cpp
        widget/field/BitmaskValueFieldWidget.cpp
        widget/field/EnumValueFieldWidget.cpp
        widget/field/StringFieldWidget.cpp
        widget/field/BitfieldFieldWidget.cpp
        widget/field/OptionalFieldWidget.cpp
        widget/field/BundleFieldWidget.cpp
        widget/field/ArrayListRawDataFieldWidget.cpp
        widget/field/ArrayListFieldWidget.cpp
        widget/field/FloatValueFieldWidget.cpp
        widget/field/VariantFieldWidget.cpp
        widget/field/UnknownValueFieldWidget.cpp
        widget/field/SpecialValueWidget.cpp
    )
    
    set (
        ui
        ui/MainWindowWidget.ui
        ui/MsgListWidget.ui
        ui/MsgDetailsWidget.ui
        ui/ProtocolsStackWidget.ui
        ui/MessageUpdateDialog.ui
        ui/RawHexDataDialog.ui
        ui/PluginsListWidget.ui
        ui/PluginConfigDialog.ui
        ui/PluginConfigWrapWidget.ui
        ui/ShortIntValueFieldWidget.ui
        ui/LongIntValueFieldWidget.ui
        ui/LongLongIntValueFieldWidget.ui
        ui/UnsignedLongLongIntValueFieldWidget.ui
        ui/BitmaskValueFieldWidget.ui
        ui/EnumValueFieldWidget.ui
        ui/StringFieldWidget.ui
        ui/BitfieldFieldWidget.ui
        ui/OptionalFieldWidget.ui
        ui/ArrayListRawDataFieldWidget.ui
        ui/ArrayListFieldWidget.ui
        ui/ArrayListElementWidget.ui
        ui/FloatValueFieldWidget.ui
        ui/VariantFieldWidget.ui
        ui/UnknownValueFieldWidget.ui
        ui/SpecialValueWidget.ui
        ui/MsgCommentDialog.ui
        ui/MessagesFilterDialog.ui
    )

    set (resources
        ui.qrc
    )
    
    set (gui_exe)
    if (WIN32 AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
        set (gui_exe WIN32)
    endif ()

    set (CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

    add_executable(${name} ${gui_exe} ${ui} ${resources} ${src})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Widgets Qt::Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${BIN_INSTALL_DIR})
        
    install (
        FILES image/app_icon.png
        DESTINATION ${ICON_INSTALL_DIR}
        RENAME cc_view.png
    )
        
endfunction ()

###########################################################

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

bin_cc_view()

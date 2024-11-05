function (lib_tools)
    set (name "${PROJECT_NAME}")
    
    set (src
        field/ToolsArrayListField.cpp
        field/ToolsBitfieldField.cpp
        field/ToolsBitmaskField.cpp
        field/ToolsBundleField.cpp
        field/ToolsEnumField.cpp
        field/ToolsFloatField.cpp
        field/ToolsIntField.cpp
        field/ToolsOptionalField.cpp
        field/ToolsRawDataField.cpp
        field/ToolsStringField.cpp
        field/ToolsUnknownField.cpp
        field/ToolsUnsignedLongField.cpp
        field/ToolsVariantField.cpp
        Message.cpp
        MessageHandler.cpp
        DataInfo.cpp
        ConfigMgr.cpp
        PluginMgr.cpp
        PluginMgrImpl.cpp
        MsgFileMgr.cpp
        MsgSendMgr.cpp
        MsgSendMgrImpl.cpp
        MsgMgr.cpp
        MsgMgrImpl.cpp
        ToolsField.cpp
        ToolsFieldHandler.cpp
        ToolsFilter.cpp
        ToolsFrame.cpp
        ToolsMsgFactory.cpp
        ToolsPlugin.cpp
        ToolsProtocol.cpp
        ToolsSocket.cpp
        property/message.cpp
    )
    
    add_library(${name} SHARED ${src})
    add_library (cc::${PROJECT_NAME} ALIAS ${name})

    target_link_libraries(${name} PUBLIC cc::comms Qt::Core ${CC_PLATFORM_SPECIFIC})
    target_include_directories(${name} BEFORE
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        INTERFACE
            $<INSTALL_INTERFACE:include>
    )

    target_compile_definitions(${name} PRIVATE -DCC_TOOLS_QT_EXPORT)    
    target_compile_options(${name} PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:/bigobj /wd4127 /wd5054 /wd5240>
    )
    
    set_target_properties(${name} PROPERTIES VERSION ${CC_TOOLS_QT_VERSION})
    
    install (
        TARGETS ${name} EXPORT ${name}Export
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    )

    if (CC_TOOLS_QT_INSTALL_LIBRARY_HEADERS)
        install(EXPORT ${name}Export NAMESPACE cc::
            DESTINATION ${LIB_INSTALL_DIR}/${INSTALL_NAME}/cmake
        )
    endif ()
    
endfunction ()

###########################################################

if (WIN32)
    if (NOT "${Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}_DIR}" STREQUAL "")
        find_library(QTPLATFORMSUPPORT_REL Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}PlatformSupport HINTS "${Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}_DIR}/lib")
        find_library(QTPLATFORMSUPPORT_DEB Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}PlatformSupportd HINTS "${Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}_DIR}/lib")
        set(QTPLATFORMSUPPORT optimized ${QTPLATFORMSUPPORT_REL} debug ${QTPLATFORMSUPPORT_DEB})
    endif ()

    if (NOT QTPLATFORMSUPPORT)
        set (QTPLATFORMSUPPORT)
    endif ()
    
    set (CC_PLATFORM_SPECIFIC ${QTPLATFORMSUPPORT} Setupapi.lib Ws2_32.lib opengl32.lib imm32.lib winmm.lib)

endif ()   

lib_tools()

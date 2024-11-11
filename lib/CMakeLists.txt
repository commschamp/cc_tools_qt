function (lib_tools)
    set (name "${PROJECT_NAME}")
    
    set (src
        src/field/ToolsArrayListField.cpp
        src/field/ToolsBitfieldField.cpp
        src/field/ToolsBitmaskField.cpp
        src/field/ToolsBundleField.cpp
        src/field/ToolsEnumField.cpp
        src/field/ToolsFloatField.cpp
        src/field/ToolsIntField.cpp
        src/field/ToolsOptionalField.cpp
        src/field/ToolsRawDataField.cpp
        src/field/ToolsStringField.cpp
        src/field/ToolsUnknownField.cpp
        src/field/ToolsUnsignedLongField.cpp
        src/field/ToolsVariantField.cpp
        src/property/message.cpp
        src/ToolsConfigMgr.cpp
        src/ToolsDataInfo.cpp
        src/ToolsField.cpp
        src/ToolsFieldHandler.cpp
        src/ToolsFilter.cpp
        src/ToolsFrame.cpp
        src/ToolsMessage.cpp
        src/ToolsMsgFactory.cpp
        src/ToolsMsgFileMgr.cpp
        src/ToolsMsgMgr.cpp
        src/ToolsMsgMgrImpl.cpp
        src/ToolsMsgSendMgr.cpp
        src/ToolsMsgSendMgrImpl.cpp
        src/ToolsPlugin.cpp
        src/ToolsPluginMgr.cpp
        src/ToolsPluginMgrImpl.cpp
        src/ToolsProtocol.cpp
        src/ToolsSocket.cpp
    )

    qt_wrap_cpp(
        moc
        include/cc_tools_qt/ToolsFilter.h
        include/cc_tools_qt/ToolsMsgSendMgr.h
        include/cc_tools_qt/ToolsPlugin.h
        include/cc_tools_qt/ToolsProtocol.h
        include/cc_tools_qt/ToolsSocket.h
    )
    
    add_library(${name} SHARED ${src} ${moc})
    add_library (cc::${PROJECT_NAME} ALIAS ${name})

    target_link_libraries(${name} PUBLIC cc::comms Qt::Core ${CC_PLATFORM_SPECIFIC})
    target_include_directories(${name} BEFORE
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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

    if (CC_TOOLS_QT_INSTALL_LIBRARY_HEADERS)
        install (
            DIRECTORY "include/cc_tools_qt"
            DESTINATION ${INC_INSTALL_DIR}
        )
    endif()     
    
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

   


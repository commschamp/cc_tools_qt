function (lib_tools)
    set (name "${PROJECT_NAME}")
    
    set (src
        ErrorStatus.cpp
        Message.cpp
        Protocol.cpp
        Filter.cpp
        Socket.cpp
        MessageHandler.cpp
        Plugin.cpp
        DataInfo.cpp
        PluginProperties.cpp
        ConfigMgr.cpp
        PluginMgr.cpp
        PluginMgrImpl.cpp
        MsgFileMgr.cpp
        MsgSendMgr.cpp
        MsgSendMgrImpl.cpp
        MsgMgr.cpp
        MsgMgrImpl.cpp
        ToolsMsgFactory.cpp
        field_wrapper/FieldWrapper.cpp
        field_wrapper/IntValueWrapper.cpp
        field_wrapper/UnsignedLongValueWrapper.cpp
        field_wrapper/BitmaskValueWrapper.cpp
        field_wrapper/EnumValueWrapper.cpp
        field_wrapper/StringWrapper.cpp
        field_wrapper/BitfieldWrapper.cpp
        field_wrapper/OptionalWrapper.cpp
        field_wrapper/BundleWrapper.cpp
        field_wrapper/ArrayListRawDataWrapper.cpp
        field_wrapper/ArrayListWrapper.cpp
        field_wrapper/FloatValueWrapper.cpp
        field_wrapper/VariantWrapper.cpp
        field_wrapper/UnknownValueWrapper.cpp
        property/message.cpp
        property/field.cpp
    )
    
    add_library(${name} SHARED ${src})
    add_library (cc::${PROJECT_NAME} ALIAS ${name})

    target_link_libraries(${name} PUBLIC cc::comms Qt::Widgets Qt::Core ${CC_PLATFORM_SPECIFIC})
    target_include_directories(${name} 
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

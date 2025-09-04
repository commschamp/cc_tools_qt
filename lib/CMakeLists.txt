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

find_package (Doxygen)
if (DOXYGEN_FOUND)
    set (doc_output_dir "${DOC_INSTALL_DIR}/cc_tools_qt")
    set (output_file "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf")

    set (out_dir_match_str "OUTPUT_DIRECTORY[^\n]*")
    set (out_dir_replacement_str "OUTPUT_DIRECTORY = ${doc_output_dir}")

    set (config_file "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen.conf")
    file (READ ${config_file} config_text)
    string (REGEX REPLACE "${out_dir_match_str}" "${out_dir_replacement_str}" modified_config_text "${config_text}")

    if (UNIX)
        set (dia_path_match_str "DIA_PATH[^\n]*")
        set (dia_path_replacement_str "DIA_PATH = ${PROJECT_SOURCE_DIR}/script")
        string (REGEX REPLACE "${dia_path_match_str}" "${dia_path_replacement_str}" modified_config_text "${modified_config_text}")
    endif ()

    file (WRITE "${output_file}" "${modified_config_text}")

    add_custom_target ("doc_cc_tools_qt"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${doc_output_dir}
            COMMAND ${DOXYGEN_EXECUTABLE} ${output_file}
            COMMAND ${CMAKE_COMMAND} -DDOC_OUTPUT_DIR="${doc_output_dir}" -P
                            ${LibComms_DIR}/CC_DocCleanupScript.cmake
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/image ${doc_output_dir}/html/image
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

#######################################################################

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




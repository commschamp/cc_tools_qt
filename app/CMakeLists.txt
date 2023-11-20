if (NOT CC_TOOLS_QT_BUILD_APPS)
    return ()
endif ()

# No RPATH for binaries
set (CMAKE_SKIP_RPATH TRUE)

add_definitions(
    -DCC_BINDIR="${CMAKE_INSTALL_BINDIR}"
    -DCC_PLUGINDIR="${PLUGIN_INSTALL_REL_DIR}"
    -DCC_CONFIGDIR="${CONFIG_INSTALL_REL_DIR}"
)

add_subdirectory (cc_view)
add_subdirectory (cc_dump)

if (WIN32 AND CC_TOOLS_QT_INSTALL_APPS)
    add_custom_target ("deploy_qt"
        COMMAND ${CMAKE_COMMAND} 
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            -DCC_BIN_DIR=${BIN_INSTALL_DIR} -DCC_PLUGIN_DIR=${PLUGIN_INSTALL_DIR}
            -P ${PROJECT_SOURCE_DIR}/cmake/CC_DeployQtScript.cmake
    )
endif ()


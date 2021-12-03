add_subdirectory (src)

if (UNIX AND CC_TOOLS_QT_INSTALL_APPS)
    install(
        PROGRAMS script/cc_view.sh
        DESTINATION ${BIN_INSTALL_DIR}
    )
endif ()  


add_subdirectory (src)

if (UNIX)
    install(
        PROGRAMS script/cc_view.sh
        DESTINATION ${BIN_INSTALL_DIR}
    )
endif ()  


add_subdirectory (src)

if (UNIX)
    install(
        PROGRAMS script/cc_dump.sh
        DESTINATION ${BIN_INSTALL_DIR}
    )
endif ()  

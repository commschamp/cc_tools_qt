add_subdirectory (src)

if (CC_TOOLS_QT_INSTALL_LIBRARY_HEADERS)
    install (
        DIRECTORY "include/cc_tools_qt"
        DESTINATION ${INC_INSTALL_DIR}
    )
endif()    


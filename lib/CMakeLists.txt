add_subdirectory (src)

if (CC_TOOLS_QT_INSTALL_LIBRARY)
    install (
        DIRECTORY "include/comms_champion"
        DESTINATION ${INC_INSTALL_DIR}
    )

    install (
        FILES ${COMMS_CHAMPION_CMAKE_FILES}
        DESTINATION ${LIB_INSTALL_DIR}/${INSTALL_NAME}/cmake/
    )
endif()    


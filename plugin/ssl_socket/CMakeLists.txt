if (Qt5Core_VERSION VERSION_LESS 5.15)
    message(WARNING "Cannot build SSL sockets, requires minimal Qt version 5.15")
    return ()
endif()

add_subdirectory (client)

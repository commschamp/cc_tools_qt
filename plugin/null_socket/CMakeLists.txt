function (plugin_null_socket)
    set (name "null_socket")
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/null_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/refresh_stamp.txt")
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/NullSocketPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    set (src
        NullSocket.cpp
        NullSocketPlugin.cpp
    )
    
    set (hdr
        NullSocketPlugin.h
    )
    
    qt5_wrap_cpp(
        moc
        ${hdr}
    )
    
    add_library (${name} MODULE ${src} ${moc})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt5::Core)
    
    if (CC_TOOLS_QT_INSTALL_PLUGINS)
        install (
            TARGETS ${name}
            DESTINATION ${PLUGIN_INSTALL_DIR})
    endif ()
    
endfunction()

######################################################################

plugin_null_socket ()

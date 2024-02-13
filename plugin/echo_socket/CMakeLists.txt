if (NOT CC_TOOLS_QT_BUILD_PLUGIN_ECHO_SOCKET)
    return()
endif ()

######################################################################

function (plugin_echo_socket)
    set (name "cc_tools_plugin_echo_socket")
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/echo_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/refresh_stamp.txt")
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/EchoSocketPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    set (src
        EchoSocket.cpp
        EchoSocketPlugin.cpp
        EchoSocketPlugin.h
    )
    
    add_library (${name} MODULE ${src})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
    
endfunction()

######################################################################

plugin_echo_socket ()

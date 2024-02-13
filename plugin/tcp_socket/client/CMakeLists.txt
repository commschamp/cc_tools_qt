
if (NOT CC_TOOLS_QT_BUILD_PLUGIN_TCP_CLIENT_SOCKET)
    return()
endif ()

######################################################################

function (plugin_tcp_client_socket)
    set (name "cc_tools_plugin_tcp_client_socket")
    
    if (NOT TARGET Qt::Network)
        message(WARNING "Can NOT build ${name} due to missing Qt::Network library")
        return()
    endif ()
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/tcp_client_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/client_refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/TcpClientSocketPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()

    set (
        ui
        TcpClientSocketConfigWidget.ui
    )    

    
    set (src
        TcpClientSocket.cpp
        TcpClientSocketPlugin.h
        TcpClientSocketPlugin.cpp
        TcpClientSocketConfigWidget.cpp
    )
    
    add_library (${name} MODULE ${ui} ${src})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Network Qt::Widgets Qt::Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
    
endfunction()

######################################################################

cc_find_qt_components (Network)

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

plugin_tcp_client_socket ()

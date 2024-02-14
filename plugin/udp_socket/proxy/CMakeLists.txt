if (NOT CC_TOOLS_QT_BUILD_PLUGIN_UDP_PROXY_SOCKET)
    return()
endif ()

######################################################################

function (plugin_udp_proxy_socket)
    set (name "cc_tools_plugin_udp_proxy_socket")
    
    if (NOT TARGET Qt::Network)
        message(WARNING "Can NOT build ${name} due to missing Qt::Network library")
        return()
    endif ()
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/udp_proxy_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/UdpProxySocketPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()

    set (
        ui
        UdpProxySocketConfigWidget.ui
    )    
    
    set (src
        UdpProxySocket.cpp
        UdpProxySocketPlugin.h
        UdpProxySocketPlugin.cpp
        UdpProxySocketConfigWidget.cpp
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

plugin_udp_proxy_socket ()

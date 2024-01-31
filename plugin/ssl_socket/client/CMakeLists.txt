
function (plugin_ssl_client_socket)
    set (name "cc_tools_plugin_ssl_client_socket")
    
    if (NOT TARGET Qt::Network)
        message(WARNING "Can NOT build ${name} due to missing Qt::Network library")
        return()
    endif ()
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/ssl_client_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/client_refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/SslClientSocketPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    set (src
        SslClientSocket.cpp
        SslClientSocketPlugin.cpp
        SslClientSocketConfigWidget.cpp
    )
    
    set (hdr
        SslClientSocket.h
        SslClientSocketPlugin.h
        SslClientSocketConfigWidget.h
    )
    
    cc_qt_wrap_cpp(
        moc
        ${hdr}
    )
    
    cc_qt_wrap_ui(
        ui
        SslClientSocketConfigWidget.ui
    )
    
    add_library (${name} MODULE ${src} ${moc} ${ui})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Network Qt::Widgets Qt::Core)
    
    if (CC_TOOLS_QT_INSTALL_PLUGINS)
        install (
            TARGETS ${name}
            DESTINATION ${PLUGIN_INSTALL_DIR})
    endif ()
    
endfunction()

######################################################################

cc_find_qt_components (Network)

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

plugin_ssl_client_socket ()

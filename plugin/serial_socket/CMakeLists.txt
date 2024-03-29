if (NOT CC_TOOLS_QT_BUILD_PLUGIN_SERIAL_SOCKET)
    return()
endif ()

######################################################################

function (plugin_serial_socket)
    set (name "cc_tools_plugin_serial_socket")
    
    if (NOT TARGET Qt::SerialPort)
        message(WARNING "Can NOT build ${name} due to missing Qt::SerialPort library")
        return()
    endif ()
    
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/serial_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/server_refresh_stamp.txt")
    
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/SerialSocketPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()

    set (
        ui
        SerialSocketConfigWidget.ui
    )
    
    set (src
        SerialSocket.cpp
        SerialSocketPlugin.h
        SerialSocketPlugin.cpp
        SerialSocketConfigWidget.cpp
        ui.qrc
    )
    
    add_library (${name} MODULE ${ui} ${src})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::SerialPort Qt::Widgets Qt::Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
    
endfunction()

######################################################################

cc_find_qt_components (SerialPort)

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

plugin_serial_socket ()

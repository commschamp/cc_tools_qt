function (cc_plugin_raw_data_protocol)
    set (name "cc_tools_plugin_raw_data_protocol")
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/raw_data_protocol.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/RawDataProtocolPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    
    set (src
        RawDataProtocolPlugin.cpp
        RawDataProtocol.cpp
        RawDataProtocolTransportMessage.cpp
        RawDataProtocolDataMessage.cpp
    )
    
    set (hdr
        RawDataProtocolPlugin.h
    )
    
    cc_qt_wrap_cpp(
        moc
        ${hdr}
    )
    
    add_library (${name} MODULE ${src} ${moc})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Core)
    
    if (CC_TOOLS_QT_INSTALL_PLUGINS)
        install (
            TARGETS ${name}
            DESTINATION ${PLUGIN_INSTALL_DIR})
    endif ()
        
endfunction()

######################################################################

cc_plugin_raw_data_protocol ()

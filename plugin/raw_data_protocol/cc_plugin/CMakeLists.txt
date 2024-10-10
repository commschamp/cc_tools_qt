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
#        RawDataProtocolPlugin.cpp
#        RawDataProtocol.cpp
#        RawDataProtocolTransportMessage.cpp
        RawDataProtocolDataMessage.cpp
#        RawDataProtocolPlugin.h
    )
    
    add_library (${name} MODULE ${src})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
        
endfunction()

######################################################################

cc_plugin_raw_data_protocol ()

function (cc_plugin_demo)
    set (name "cc_tools_plugin_demo_protocol")
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/demo.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/demo_refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/DemoPlugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    
    set (src
        DemoPlugin.h
        DemoPlugin.cpp
        DemoProtocol.cpp
        DemoTransportMessage.cpp
        DemoMessage.cpp
        message/IntValues.cpp
        message/EnumValues.cpp
        message/BitmaskValues.cpp
        message/Bitfields.cpp
        message/Strings.cpp
        message/Lists.cpp
        message/Optionals.cpp
        message/FloatValues.cpp
        message/Variants.cpp
        message/Bundles.cpp
    )
    
    add_library (${name} MODULE ${src})
    target_link_libraries(${name} PRIVATE cc::demo cc::cc_tools_qt)

    target_include_directories(${name} 
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )    
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
        
endfunction()

######################################################################


cc_plugin_demo ()


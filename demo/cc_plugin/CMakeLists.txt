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
    target_link_libraries(${name} PRIVATE cc::cc_tools_qt)

    target_compile_options(${name} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wno-old-style-cast>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-old-style-cast>
    )      
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
        
endfunction()

######################################################################

include_directories(
    ${PROJECT_SOURCE_DIR}/lib/include)

cc_plugin_demo ()

FILE(GLOB_RECURSE headers "*.h")
add_custom_target(demo.cc_plugin_headers SOURCES ${headers})

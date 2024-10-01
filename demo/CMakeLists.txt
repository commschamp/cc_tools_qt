if (NOT CC_TOOLS_QT_BUILD_PLUGIN_DEMO_PROTOCOL)
    return ()
endif ()

add_subdirectory (protocol)
add_subdirectory (cc_plugin)

return ()

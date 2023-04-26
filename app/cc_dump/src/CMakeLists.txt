function (bin_cc_dump)
    set (name "cc_dump")
    
    set (src
        main.cpp
        AppMgr.cpp
        CsvDumpMessageHandler.cpp
        RecordMessageHandler.cpp
    )
    
    cc_qt_wrap_cpp(
        moc
        AppMgr.h
    )
    
    add_executable(${name} ${src} ${moc})
    target_link_libraries(${name} PRIVATE cc::${PROJECT_NAME} Qt::Core)
    
    if (CC_TOOLS_QT_INSTALL_APPS)
        install (
            TARGETS ${name}
            DESTINATION ${BIN_INSTALL_DIR})
    endif ()
        
endfunction ()

###########################################################

include_directories (
#    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

bin_cc_dump()

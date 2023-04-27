macro (cc_find_qt_major)
    find_package(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION} REQUIRED COMPONENTS Core Widgets)
    message (STATUS "Using Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}")

    if (NOT TARGET Qt::Core)
        add_library(Qt::Core ALIAS Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::Core)
        set_target_properties(Qt::Core PROPERTIES IMPORTED_GLOBAL TRUE)
    endif ()

    if (NOT TARGET Qt::Widgets)
        add_library(Qt::Widgets ALIAS Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::Widgets)
        set_target_properties(Qt::Widgets PROPERTIES IMPORTED_GLOBAL TRUE)
    endif ()    
endmacro ()

macro (cc_find_qt_components)
    find_package(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION} OPTIONAL_COMPONENTS ${ARGN})

    foreach (c ${ARGN})
        if (NOT TARGET Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::${c})
            continue ()
        endif ()

        if (NOT TARGET Qt::${c})
            add_library(Qt::${c} ALIAS Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::${c})
            set_target_properties(Qt::${c} PROPERTIES IMPORTED_GLOBAL TRUE)
        endif ()        
    endforeach ()
endmacro ()

macro (cc_qt_wrap_cpp _name)
    if ("${CC_TOOLS_QT_MAJOR_QT_VERSION}" STREQUAL "5")
        qt5_wrap_cpp(${_name} ${ARGN})
    else ()
        qt_wrap_cpp(${_name} ${ARGN})
    endif ()
endmacro ()

macro (cc_qt_wrap_ui _name)
    if ("${CC_TOOLS_QT_MAJOR_QT_VERSION}" STREQUAL "5")
        qt5_wrap_ui(${_name} ${ARGN})
    else ()
        qt_wrap_ui(${_name} ${ARGN})
    endif ()
endmacro ()

macro (cc_qt_add_resources _name)
    if ("${CC_TOOLS_QT_MAJOR_QT_VERSION}" STREQUAL "5")
        qt5_add_resources(${_name} ${ARGN})
    else ()
        qt_add_resources(${_name} ${ARGN})
    endif ()
endmacro ()


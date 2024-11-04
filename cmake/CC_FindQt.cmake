macro (cc_find_qt_major)
    set (qt_components Core)
    if (CC_TOOLS_QT_BUILD_APPS)
        list (APPEND qt_components Widgets)
    endif ()

    find_package(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION} REQUIRED COMPONENTS ${qt_components})
    message (STATUS "Using Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}")

    if (NOT TARGET Qt::Core)
        set_target_properties(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::Core PROPERTIES IMPORTED_GLOBAL TRUE)
        add_library(Qt::Core ALIAS Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::Core)
    endif ()

    if (NOT TARGET Qt::Widgets)
        set_target_properties(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::Widgets PROPERTIES IMPORTED_GLOBAL TRUE)
        add_library(Qt::Widgets ALIAS Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::Widgets)
    endif ()    
endmacro ()

macro (cc_find_qt_components)
    find_package(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION} OPTIONAL_COMPONENTS ${ARGN})

    foreach (c ${ARGN})
        if (NOT TARGET Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::${c})
            continue ()
        endif ()

        if (NOT TARGET Qt::${c})
            set_target_properties(Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::${c} PROPERTIES IMPORTED_GLOBAL TRUE)
            add_library(Qt::${c} ALIAS Qt${CC_TOOLS_QT_MAJOR_QT_VERSION}::${c})
        endif ()        
    endforeach ()
endmacro ()

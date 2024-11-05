# - Find cc_tools_qt
# Find the cc_tools_qt includes and libraries
# 
# Output is provided using the following variables
#  CC_TOOLS_QT_FOUND - Found library, use CC_TOOLS_QT_INCLUDE_DIRS, CC_TOOLS_QT_PLUGIN_LIBRARIES, CC_TOOLS_QT_PLUGIN_DIR)
#  CC_TOOLS_QT_INCLUDE_DIRS - Where to find all the headers
#  CC_TOOLS_QT_ROOT_DIR - Root directory of cc_tools_qt installation
#  CC_TOOLS_QT_PLUGIN_LIBRARIES - Libraries to link against when building a plugin
#  CC_TOOLS_QT_PLUGIN_LIBRARY_DIRS - Where to find the libraries required to build plugin.
#  CC_TOOLS_QT_PLUGIN_DIR - Directory where to install newly built plugin
#
# Additional output targets
# cc::cc_tools_qt - Link target for "cc_tools_qt" libarary. Use it in plugin development.

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/cc_tools_qtExport.cmake)
    include (${CMAKE_CURRENT_LIST_DIR}/cc_tools_qtExport.cmake)
endif ()

if (NOT TARGET cc::cc_tools_qt)
    return ()
endif ()

# Load information for each installed configuration.
file(GLOB config_files "${CMAKE_CURRENT_LIST_DIR}/cc_tools_qtConfig-*.cmake")
foreach(f ${config_files})
    include(${f})
endforeach()

get_filename_component (CC_TOOLS_QT_INSTALL_LIB_PROJ_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
get_filename_component (CC_TOOLS_QT_INSTALL_LIB_DIR ${CC_TOOLS_QT_INSTALL_LIB_PROJ_DIR} DIRECTORY)
get_filename_component (CC_TOOLS_QT_ROOT_DIR ${CC_TOOLS_QT_INSTALL_LIB_DIR} DIRECTORY)

find_path(CC_TOOLS_QT_INCLUDE_DIR NAMES cc_tools_qt/ToolsMessage.h PATHS "${CC_TOOLS_QT_ROOT_DIR}" PATH_SUFFIXES include)
find_library(CC_TOOLS_QT_PLUGIN_LIBRARY  NAMES "cc_tools_qt" PATHS "${CC_TOOLS_QT_ROOT_DIR}" PATH_SUFFIXES lib)

if (MSVC)
    find_file(CC_NULL_SOCK_LIBRARY NAMES "null_socket.dll" PATHS "${CC_TOOLS_QT_ROOT_DIR}/lib/cc_tools_qt" PATH_SUFFIXES plugin)
else ()
    find_library(CC_NULL_SOCK_LIBRARY  NAMES "null_socket" PATHS "${CC_TOOLS_QT_ROOT_DIR}/lib/cc_tools_qt" PATH_SUFFIXES plugin)
endif ()

if (CC_TOOLS_QT_PLUGIN_LIBRARY)
    get_filename_component (CC_TOOLS_QT_PLUGIN_LIBRARY_DIR ${CC_TOOLS_QT_PLUGIN_LIBRARY} DIRECTORY)
endif ()

if (CC_NULL_SOCK_LIBRARY)
    get_filename_component  (CC_TOOLS_QT_PLUGIN_DIR ${CC_NULL_SOCK_LIBRARY} DIRECTORY)
endif ()

find_package(PackageHandleStandardArgs REQUIRED)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    cc_tools_qt
    REQUIRED_VARS 
        CC_TOOLS_QT_ROOT_DIR CC_TOOLS_QT_INCLUDE_DIR CC_TOOLS_QT_PLUGIN_LIBRARY_DIR CC_TOOLS_QT_PLUGIN_LIBRARY)

set (CC_TOOLS_QT_FOUND ${cc_tools_qt_FOUND})        
    
if (CC_TOOLS_QT_FOUND)
    set (CC_TOOLS_QT_INCLUDE_DIRS ${CC_TOOLS_QT_INCLUDE_DIR})
    set (CC_TOOLS_QT_PLUGIN_LIBRARIES ${CC_TOOLS_QT_PLUGIN_LIBRARY})
    set (CC_TOOLS_QT_PLUGIN_LIBRARY_DIRS ${CC_TOOLS_QT_PLUGIN_LIBRARY_DIR})
endif ()    


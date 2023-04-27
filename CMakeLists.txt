cmake_minimum_required (VERSION 3.11)
project ("cc_tools_qt")

# Available options

option (CC_TOOLS_QT_INSTALL_LIBRARY_HEADERS "Install tools library headers." ON)
option (CC_TOOLS_QT_BUILD_APPS "Build tool applications." ON)
option (CC_TOOLS_QT_INSTALL_APPS "Install tools binaries." ${CC_TOOLS_QT_BUILD_APPS})
option (CC_TOOLS_QT_BUILD_PLUGINS "Build plugins." ${CC_TOOLS_QT_BUILD_APPS})
option (CC_TOOLS_QT_INSTALL_PLUGINS "Install plugins." ${CC_TOOLS_QT_BUILD_PLUGINS})
option (CC_TOOLS_QT_BUILD_DEMO_PROTOCOL "Build demo protocol plugin." OFF)
option (CC_TOOLS_QT_INSTALL_DEMO_PROTOCOL "Install demo protocol plugin." ${CC_TOOLS_QT_BUILD_DEMO_PROTOCOL})
option (CC_TOOLS_QT_WARN_AS_ERR "Treat warning as error" ON)
option (CC_TOOLS_QT_USE_CCACHE "Use ccache on UNIX systems if it's available" ON)
option (CC_TOOLS_QT_SKIP_CXX_STANDARD_FORCING "Do NOT force C++ standard to C++11, use compiler's default one." ON)
option (CC_TOOLS_QT_STATIC_RUNTIME "Enable/Disable static runtime" OFF)
option (CC_TOOLS_QT_USE_QT5 "Don't look for Qt6" OFF)

# Extra configuration variables
set (CC_TOOLS_QT_MAJOR_QT_VERSION 5 CACHE STRING "Major Qt version")

#######################################################################

if (CMAKE_TOOLCHAIN_FILE AND EXISTS ${CMAKE_TOOLCHAIN_FILE})
    message(STATUS "Loading toolchain from ${CMAKE_TOOLCHAIN_FILE}")
endif()

set (CMAKE_SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/cmake")

if (("${CMAKE_CXX_STANDARD}" STREQUAL "") AND (NOT CC_COMMS_SKIP_CXX_STANDARD_FORCING))
    set (CMAKE_CXX_STANDARD 11)
endif()

include(GNUInstallDirs)
set (INSTALL_NAME "cc_tools_qt")
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR})
set (BIN_INSTALL_DIR ${CMAKE_INSTALL_FULL_BINDIR})
set (INC_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set (PLUGIN_INSTALL_REL_DIR ${CMAKE_INSTALL_LIBDIR}/${INSTALL_NAME}/plugin)
set (PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${PLUGIN_INSTALL_REL_DIR})
set (DATA_INSTALL_REL_DIR ${CMAKE_INSTALL_DATAROOTDIR}/${INSTALL_NAME})
set (DATA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${DATA_INSTALL_REL_DIR})
set (DOC_INSTALL_DIR ${CMAKE_INSTALL_FULL_DATAROOTDIR}/doc)
set (ICON_INSTALL_DIR ${DATA_INSTALL_DIR}/icon)
set (CONFIG_INSTALL_REL_DIR ${CMAKE_INSTALL_DATADIR}/${INSTALL_NAME})
set (CONFIG_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${CONFIG_INSTALL_REL_DIR})

file (READ "${PROJECT_SOURCE_DIR}/lib/include/cc_tools_qt/version.h" version_file)
string (REGEX MATCH "CC_TOOLS_QT_MAJOR_VERSION ([0-9]*)U*" _ ${version_file})
set (major_ver ${CMAKE_MATCH_1})
string (REGEX MATCH "CC_TOOLS_QT_MINOR_VERSION ([0-9]*)U*" _ ${version_file})
set (minor_ver ${CMAKE_MATCH_1})
string (REGEX MATCH "CC_TOOLS_QT_PATCH_VERSION ([0-9]*)U*" _ ${version_file})
set (patch_ver ${CMAKE_MATCH_1})
set (CC_TOOLS_QT_VERSION "${major_ver}.${minor_ver}.${patch_ver}")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cc_tools_qtVersion.cmake
    VERSION ${CC_TOOLS_QT_VERSION}
    COMPATIBILITY AnyNewerVersion)

#######################################################################

find_package(LibComms REQUIRED NO_MODULE)

#######################################################################

# Compiler options
set (warn_opt)
if (CC_TOOLS_QT_WARN_AS_ERR)
    set (warn_opt WARN_AS_ERR)
endif ()

set (static_runtime_opt)
if (CC_TOOLS_QT_STATIC_RUNTIME)
    set (static_runtime_opt STATIC_RUNTIME)
endif ()

set (ccache_opt)
if ((UNIX) AND (CC_TOOLS_QT_USE_CCACHE))
    set (ccache_opt USE_CCACHE)
endif ()

include (${LibComms_DIR}/CC_Compile.cmake)
cc_compile(${warn_opt} ${static_runtime_opt} ${ccache_opt})
cc_msvc_force_warn_opt("/W4")

#######################################################################

find_package (Doxygen)
if (DOXYGEN_FOUND)
    set (doc_output_dir "${DOC_INSTALL_DIR}/cc_tools_qt")
    set (match_str "OUTPUT_DIRECTORY[^\n]*")
    set (replacement_str "OUTPUT_DIRECTORY = ${doc_output_dir}")
    set (output_file "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf")

    set (config_file "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen.conf")
    file (READ ${config_file} config_text)
    string (REGEX REPLACE "${match_str}" "${replacement_str}" modified_config_text "${config_text}")
    file (WRITE "${output_file}" "${modified_config_text}")

    add_custom_target ("doc_cc_tools_qt"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${doc_output_dir}
            COMMAND ${DOXYGEN_EXECUTABLE} ${output_file}
            COMMAND ${CMAKE_COMMAND} -DDOC_OUTPUT_DIR="${doc_output_dir}" -P 
                            ${LibComms_DIR}/CC_DocCleanupScript.cmake
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
                    ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/image ${doc_output_dir}/html/image
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

#######################################################################

if (CC_TOOLS_QT_INSTALL_LIBRARY_HEADERS)
    set (LIB_CC_TOOLS_QT_CMAKE_FILES
        ${PROJECT_SOURCE_DIR}/cmake/cc_tools_qtConfig.cmake
        ${CMAKE_BINARY_DIR}/cc_tools_qtVersion.cmake
    )

    install (
        FILES ${LIB_CC_TOOLS_QT_CMAKE_FILES}
        DESTINATION ${LIB_INSTALL_DIR}/cc_tools_qt/cmake/
    )
endif ()

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/include
)

#######################################################################

include (${PROJECT_SOURCE_DIR}/cmake/CC_FindQt.cmake)
cc_find_qt_major ()

add_subdirectory (lib)
add_subdirectory (plugin)
add_subdirectory (app)
add_subdirectory (demo)


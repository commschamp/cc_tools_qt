cmake_minimum_required (VERSION 3.1)
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
option (CC_TOOLS_QT_EXTERNAL_COMMS "External COMMS Library" OFF)

# Extra variables
# CC_TOOLS_QT_DIR=dir - Directory of QT5 installation. Can be used to provide path to QT5 if
#   differs from system default installation path.
# CC_TOOLS_QT_EXTERNALS_DIR - Directory where pull externals, defaults to 
#   ${PROJECT_SOURCE_DIR}/externals
# COMMS_TAG - Tag/Branch of the COMMS library to checkout if needed. 

#######################################################################

if (CMAKE_TOOLCHAIN_FILE AND EXISTS ${CMAKE_TOOLCHAIN_FILE})
    message(STATUS "Loading toolchain from ${CMAKE_TOOLCHAIN_FILE}")
endif()

if (NOT CC_TOOLS_QT_EXTERNALS_DIR)
    set (CC_TOOLS_QT_EXTERNALS_DIR "${PROJECT_SOURCE_DIR}/externals")
endif ()

set (CMAKE_SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/cmake")

if (("${CMAKE_CXX_STANDARD}" STREQUAL "") AND (NOT CC_COMMS_SKIP_CXX_STANDARD_FORCING))
    set (CMAKE_CXX_STANDARD 11)
endif()

if ("${COMMS_TAG}" STREQUAL "")
    set (COMMS_TAG "develop")
endif ()

include(GNUInstallDirs)
set (INSTALL_NAME "cc_tools_qt")
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
set (BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
set (INC_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set (PLUGIN_INSTALL_REL_DIR ${CMAKE_INSTALL_LIBDIR}/${INSTALL_NAME}/plugin)
set (PLUGIN_INSTALL_DIR ${PLUGIN_INSTALL_REL_DIR})
set (DATA_INSTALL_REL_DIR ${CMAKE_INSTALL_DATAROOTDIR}/${INSTALL_NAME})
set (DATA_INSTALL_DIR ${DATA_INSTALL_REL_DIR})
set (DOC_INSTALL_DIR ${CMAKE_INSTALL_DATAROOTDIR}/doc)
set (ICON_INSTALL_DIR ${DATA_INSTALL_DIR}/icon)
set (CONFIG_INSTALL_REL_DIR ${CMAKE_INSTALL_DATADIR}/${INSTALL_NAME})
set (CONFIG_INSTALL_DIR ${CONFIG_INSTALL_REL_DIR})

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
# Get COMMS library target

if (NOT CC_TOOLS_QT_EXTERNAL_COMMS)
    set (comms_src_dir "${CC_TOOLS_QT_EXTERNALS_DIR}/comms")
    include (${CMAKE_SCRIPTS_DIR}/CC_CommsPrefetch.cmake)
    cc_comms_prefetch(SRC_DIR "${comms_src_dir}" TAG "${COMMS_TAG}")

    set (CC_COMMS_CMAKE_DIR "${comms_src_dir}/cmake")
    include (${CC_COMMS_CMAKE_DIR}/CC_CommsExternal.cmake)

    set (comms_build_dir "${CMAKE_CURRENT_BINARY_DIR}/comms")
    set (comms_install_dir "${comms_build_dir}/install")
    cc_comms_build_during_config(
        NO_REPO
        SRC_DIR "${comms_src_dir}"
        BUILD_DIR ${comms_build_dir}
        CMAKE_ARGS -DCC_COMMS_BUILD_UNIT_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${comms_install_dir}
    )

    list (APPEND CMAKE_PREFIX_PATH ${comms_install_dir})
endif ()

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

if ("${CC_COMMS_CMAKE_DIR}" STREQUAL "")
    message (FATAL_ERROR "CC_COMMS_CMAKE_DIR variable is not set")
endif ()

include (${CC_COMMS_CMAKE_DIR}/CC_Compile.cmake)
cc_compile(${warn_opt} ${static_runtime_opt} ${ccache_opt})
cc_msvc_force_warn_opt("/W4")

#######################################################################

find_package (Doxygen)
if (DOXYGEN_FOUND)
    set (doc_output_dir "${CMAKE_INSTALL_PREFIX}/${DOC_INSTALL_DIR}/cc_tools_qt")
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
                            ${CC_COMMS_CMAKE_DIR}/CC_DocCleanupScript.cmake
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
while (TRUE)
    if (NOT WIN32)
        break()
    endif ()

    if (CC_TOOLS_QT_DIR)
        break ()
    endif ()

    if (QT_DIR)
        set (CC_TOOLS_QT_DIR ${CC_TOOLS_QT_DIR}) 
    endif ()

    if (Qt5_DIR)
        set (CC_TOOLS_QT_DIR ${Qt5_DIR}) 
    endif ()    
    break()
endwhile ()

if (CC_TOOLS_QT_DIR)
    list (APPEND CMAKE_PREFIX_PATH ${CC_TOOLS_QT_DIR})
endif ()
#######################################################################

add_subdirectory (lib)
add_subdirectory (plugin)
add_subdirectory (app)
add_subdirectory (demo)


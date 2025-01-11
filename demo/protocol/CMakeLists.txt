add_library(demo INTERFACE)
add_library (cc::demo ALIAS demo)

target_include_directories(demo INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
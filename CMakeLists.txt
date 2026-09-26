cmake_minimum_required(VERSION 3.20)
project(uSoralis2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USORALIS_ENABLE_CLANG_TIDY "Enable clang-tidy during compilation" OFF)

if(USORALIS_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy-22 clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
endif()

add_subdirectory(third_party/TinyReguMath3D EXCLUDE_FROM_ALL)

add_library(usoralis INTERFACE)
target_include_directories(usoralis INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(usoralis INTERFACE TinyReguMath3D)

file(GLOB_RECURSE USORALIS_FORMAT_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example/*.cpp"
)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format-22 clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${USORALIS_FORMAT_FILES}
        COMMAND_EXPAND_LISTS
        VERBATIM
    )
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${USORALIS_FORMAT_FILES}
        COMMAND_EXPAND_LISTS
        VERBATIM
    )
else()
    message(STATUS "clang-format was not found; format targets are disabled")
endif()

include(CTest)
add_subdirectory(example/bmp)

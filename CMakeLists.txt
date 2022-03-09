# CMake
cmake_minimum_required(VERSION 3.21)
project(Genode)

# Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find library
find_package(SFML 2.5.1 COMPONENTS audio graphics system window REQUIRED)

# Main Sources
file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)
file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE HEADERS extlibs/headers/*.hpp)
include_directories(src)
include_directories(include)
include_directories(extlibs/headers)

# Library Constants and Sources
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${SFML_ROOT}/cmake/SFML")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
include_directories(${SFML_INCLUDE_DIR})

# Build Options
option(BUILD_SHARED_LIBS "Build project as shared libraries" OFF)

# Executable
set(LIBRARY_NAME "Genode.IO")
add_library(${LIBRARY_NAME} ${SRCS} ${HEADERS})

# Linking Libraries
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} sfml-audio sfml-graphics sfml-system sfml-window)

# OS-Specific Configuration
if(WIN32)
    # Libraries flags
    set(SFML_STATIC_LIBRARIES TRUE)

    # Compiler Flags (MTd)
    set_property(TARGET ${LIBRARY_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Output directory
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/windows/${CMAKE_BUILD_TYPE}")
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    set_target_properties(${LIBRARY_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${OUTPUT_DIR}
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG  ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}
    )
elseif(APPLE)
    # Output Directory
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/osx/${CMAKE_BUILD_TYPE}")
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
elseif(UNIX)
    # Compiler Flags
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
        set(CMAKE_C_COMPILER gcc)
        set(CMAKE_CXX_COMPILER g++)
    else()
        set(CMAKE_C_COMPILER gcc-8)
        set(CMAKE_CXX_COMPILER g++-8)
    endif()
    set(CMAKE_C_FLAGS "-lstdc++fs -std=c++17")
    set(CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

    # Output Directory
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/linux/${CMAKE_BUILD_TYPE}")
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
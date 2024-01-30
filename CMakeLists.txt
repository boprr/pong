
cmake_minimum_required(VERSION 3.7)
project(pong)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(pong ${SDL2_INCLUDE_DIRS})

# STATIC stuff (Windows)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(BUILD_FOR_WIN TRUE)
endif()
option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})

if (STATIC_BUILD)
        set(CMAKE_EXE_LINKER_FLAGS "-static")
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
        set(BUILD_SHARED_LIBS OFF)
endif()

file(GLOB src CONFIGURE_DEPENDS "src/*.h" "src/*.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_C_FLAGS "-O2")
include_directories(pong ${SDL2_INCLUDE_DIRS})
add_executable(pong ${src})
target_link_libraries(pong -lSDL2_ttf -lm ${SDL2_LIBRARIES})

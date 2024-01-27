
cmake_minimum_required(VERSION 3.7)
project(pong)

find_package(SDL2 REQUIRED)
include_directories(pong ${SDL2_INCLUDE_DIRS})

file(GLOB src CONFIGURE_DEPENDS "src/*.h" "src/*.c")
add_executable(pong ${src})
target_link_libraries(pong -lm ${SDL2_LIBRARIES})

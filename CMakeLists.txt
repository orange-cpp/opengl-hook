cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")

project(seville)
add_library(seville MODULE source/main.cpp)

add_subdirectory(extlibs)

target_link_libraries(seville PRIVATE opengl32 imgui minhook)
target_compile_definitions(seville PRIVATE WIN32_LEAN_AND_MEAN)
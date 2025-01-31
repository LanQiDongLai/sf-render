cmake_minimum_required(VERSION 3.20)

project(sf-render)

file(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_executable(${PROJECT_NAME} main.cc ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} SDL2)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

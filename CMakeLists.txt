cmake_minimum_required(VERSION 3.31)
project(ctest)
add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendored/sdl)


target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)
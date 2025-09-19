cmake_minimum_required(VERSION 3.31)
project(ctest)

set(SDL_STATIC ON)
add_subdirectory(vendor/sdl)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)
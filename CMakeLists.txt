cmake_minimum_required(VERSION 3.31)
project(ctest)

set(SDL_STATIC ON)
add_subdirectory(extern/SDL)

add_executable(${PROJECT_NAME} src/main.cpp)

if (WIN32) 
  set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE "$<$<CONFIG:Release>:TRUE>"
  )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)
target_include_directories(${PROJECT_NAME} 
  PRIVATE 
    ${SDL3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
) 

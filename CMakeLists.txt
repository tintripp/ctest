cmake_minimum_required(VERSION 3.21)
project(ctest)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.c")
add_executable(${PROJECT_NAME} ${SRC_FILES})

if (WIN32) 
  set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE "$<$<CONFIG:Release>:TRUE>"
  )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
  SDL2 
  SDL2main
  SDL2_image
  SDL2_mixer
  SDL2_ttf
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${SDL2_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
)

add_custom_command(
  TARGET ${PROJECT_NAME} 
  POST_BUILD 
  COMMAND 
    ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_CURRENT_SOURCE_DIR}/res/"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res/"
  COMMENT "Copying resource file"
)
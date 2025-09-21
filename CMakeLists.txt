cmake_minimum_required(VERSION 3.31)
project(ctest)

# set(SDL_STATIC ON)
add_subdirectory(extern/SDL)
add_subdirectory(extern/SDL_image)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

if (WIN32) 
  set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE "$<$<CONFIG:Release>:TRUE>"
  )
endif()

target_link_libraries(${PROJECT_NAME} 
  PRIVATE SDL3_image::SDL3_image 
  SDL3::SDL3
)#-static)

target_include_directories(${PROJECT_NAME} 
  PRIVATE 
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
cmake_minimum_required(VERSION 3.20)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_subdirectory(external/GLFW)
add_subdirectory(external/glad)

add_executable(${PROJECT_NAME} 
src/main.cpp
src/shader-program.cpp
src/shader-builder.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw opengl32)

target_include_directories(${PROJECT_NAME} PRIVATE
    external/glfw/include
    external/glad/include
)

target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wextra -Wpedantic)

# Post-build step to copy the executable to the top-level directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.exe
)

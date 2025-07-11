cmake_minimum_required(VERSION 3.20)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_subdirectory(external/GLFW)
add_subdirectory(external/glad)
add_subdirectory(external/stb_image)
add_subdirectory(external/whereami)
add_subdirectory(external/spdlog)

add_executable(${PROJECT_NAME} 
src/main.cpp
src/shader-builder.cpp
src/vao-builder.cpp
src/opengl-debug.cpp
src/asset-path-resolver.cpp
src/texture-builder.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw opengl32 stb_image whereami spdlog)

target_include_directories(${PROJECT_NAME} PRIVATE
    external/glfw/include
    external/glad/include
    external/stb_image/include
    external/whereami/src
    external/spdlog/include
)

target_compile_options(${PROJECT_NAME} PRIVATE -s -Wall -Wextra -Wpedantic)

# Post-build step to copy the executable to the top-level directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.exe
)

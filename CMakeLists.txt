cmake_minimum_required(VERSION 3.10...3.27)
project(openfight)
set(LIBRARY_NAME libopenfight)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB LIBOPENFIGHT_SOURCE CONFIGURE_DEPENDS src/libopenfight/*.cpp)
file(GLOB LIBOPENFIGHT_HEADER CONFIGURE_DEPENDS src/libopenfight/*.h)
file(GLOB SDL_SOURCE CONFIGURE_DEPENDS src/platform/sdl/*.cpp)

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLU REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
find_package(yaml-cpp REQUIRED)

if(APPLE)
    find_package(GLEW REQUIRED CONFIG)
else()
    find_package(GLEW REQUIRED)
endif()

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
)

add_library(${LIBRARY_NAME} ${LIBOPENFIGHT_SOURCE} ${LIBOPENFIGHT_HEADER})
set_target_properties(${LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${LIBRARY_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src/libopenfight
)

target_link_libraries(
    ${LIBRARY_NAME}
    yaml-cpp
)

add_executable(${PROJECT_NAME} ${SDL_SOURCE})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/platform/sdl
        ${OPENGL_INCLUDE_DIR}
        ${GLU_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${LIBRARY_NAME}
    ${OPENGL_gl_LIBRARY}
    ${GLU_LIBRARY}
    ${GLEW_LIBRARIES}
    GLEW::GLEW
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_CURRENT_BINARY_DIR}/..
)

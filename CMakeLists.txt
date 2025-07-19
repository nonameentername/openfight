cmake_minimum_required(VERSION 3.10...3.27)
project(openfight)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER src/*.h)

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLU REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
find_package(LibXml2 REQUIRED)

if(APPLE)
    find_package(GLEW REQUIRED CONFIG)
else()
    find_package(GLEW REQUIRED)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${GLU_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
)

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER})

target_link_libraries(
    ${PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    ${GLU_LIBRARY}
    ${GLEW_LIBRARIES}
    GLEW::GLEW
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${LIBXML2_LIBRARIES}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_CURRENT_BINARY_DIR}/..
)

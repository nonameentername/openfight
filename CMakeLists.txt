project(openfight)

cmake_minimum_required(VERSION 3.5)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER src/*.h)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(LibXml2 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDL_IMAGE_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR})

add_executable(openfight ${SOURCE} ${HEADER})

target_link_libraries(
    openfight
    ${OPENGL_LIBRARY}
    ${SDL_LIBRARY}
    ${SDL_IMAGE_LIBRARIES}
    ${LIBXML2_LIBRARIES}
)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

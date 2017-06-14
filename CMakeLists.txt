project(sdl_game)

cmake_minimum_required(VERSION 2.6)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER src/*.h)

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

add_executable(sdl_game ${SOURCE} ${HEADER})

target_link_libraries(
    sdl_game
    ${OPENGL_LIBRARY}
    ${SDL_LIBRARY}
    ${SDL_IMAGE_LIBRARIES}
    ${LIBXML2_LIBRARIES})

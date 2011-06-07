project(sdl_game)

cmake_minimum_required(VERSION 2.6)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER src/*.h)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(LibXml2 REQUIRED)

add_definitions(`sdl-config --cflags` `xml2-config --cflags`)

link_libraries(-lxml2 -lSDL -lSDL_image -lGL -lGLU)

add_executable(sdl_game ${SOURCE} ${HEADER})

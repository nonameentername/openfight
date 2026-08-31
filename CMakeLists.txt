cmake_minimum_required(VERSION 3.10...3.27)

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "" FORCE)

set(VCPKG_FEATURE_FLAGS versions)
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_SOURCE_DIR}/platform/vcpkg/triplets)
set(VCPKG_OVERLAY_PORTS ${CMAKE_CURRENT_SOURCE_DIR}/platform/vcpkg/ports)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/modules/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

if(OSXCROSS_TARGET)
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
        set(VCPKG_TARGET_TRIPLET "x64-osxcross")
        set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/platform/osxcross/osx-x64.cmake)
    elseif(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        set(VCPKG_TARGET_TRIPLET "arm64-osxcross")
        set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/platform/osxcross/osx-arm64.cmake)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(GODOT_LIBRARIES_SUFFIX ".macos.editor.dev.${CMAKE_OSX_ARCHITECTURES}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
    else()
        set(GODOT_LIBRARIES_SUFFIX ".macos.editor.${CMAKE_OSX_ARCHITECTURES}")
    endif()
    set(OSXCROSS_FRAMEWORKS
        "-framework Cocoa"
        "-framework Foundation"
        "-framework CoreFoundation"
        "-framework CoreAudio"
        "-framework AudioToolbox"
        "-framework CoreVideo"
        "-framework CoreGraphics"
        "-framework IOKit"
        "-framework ForceFeedback"
        "-framework GameController"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore"
        "-framework CoreHaptics"
        "-framework Carbon"
        "-framework ApplicationServices"
        "-framework Security"
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(VCPKG_TARGET_TRIPLET "wasm32-emscripten")
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/platform/web/wasm32.cmake)
    set(VCPKG_CMAKE_SYSTEM_NAME Emscripten)
elseif (CMAKE_SYSTEM_NAME STREQUAL "MinGW" AND UNIX)
    set(VCPKG_TARGET_TRIPLET "x64-mingw-static")
    set(VCPKG_INSTALL_OPTIONS "--allow-unsupported")
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/modules/vcpkg/scripts/toolchains/mingw.cmake)
    set(VCPKG_CMAKE_SYSTEM_NAME MinGW)
    set(VCPKG_TARGET_ARCHITECTURE x64)
elseif(UNIX)
    set(VCPKG_TARGET_TRIPLET "x64-linux")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(GODOT_LIBRARIES_SUFFIX ".linuxbsd.editor.dev.x86_64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
    else()
        set(GODOT_LIBRARIES_SUFFIX ".linuxbsd.editor.x86_64")
    endif()
endif()

project(openfight)

message(STATUS "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

set(LIBRARY_NAME libopenfight)

if(ENABLE_ASAN AND CMAKE_BUILD_TYPE MATCHES Debug)
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

if(NOT OSXCROSS_TARGET)
    list(REMOVE_ITEM SDL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/sdl/osxcrossAvailability.cpp)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    list(REMOVE_ITEM SDL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/sdl/sdlGlRenderBackend.cpp)
    list(REMOVE_ITEM SDL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/sdl/main.cpp)
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLU REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
find_package(yaml-cpp REQUIRED)

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
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

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
add_executable(${PROJECT_NAME} ${SDL_SOURCE})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/platform/sdl
        ${OPENGL_INCLUDE_DIR}
        ${GLU_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${LIBRARY_NAME}
    ${OPENGL_gl_LIBRARY}
    ${GLU_LIBRARY}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OSXCROSS_FRAMEWORKS}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_CURRENT_BINARY_DIR}/..
)
endif()


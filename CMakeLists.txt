cmake_minimum_required(VERSION 2.8)
project(sdl2_rendercopyex_demo)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR})

add_executable(sdl2_rendercopyex_demo main.c)

# libSDLMain comes with libSDL if needed on certain platforms
if(NOT SDLMAIN_LIBRARY)
  set(SDLMAIN_LIBRARY "")
endif()

target_link_libraries(sdl2_rendercopyex_demo
  ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDLMAIN_LIBRARY})

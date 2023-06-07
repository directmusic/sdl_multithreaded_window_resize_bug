cmake_minimum_required(VERSION 3.13.2)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version" FORCE)

project(SDL_RESIZE_TEST LANGUAGES C CXX VERSION 1.0.0)

add_subdirectory(external/SDL)

add_executable(SDL_RESIZE_TEST main.c)

find_package(Vulkan REQUIRED)
target_include_directories(SDL_RESIZE_TEST PRIVATE ${CMAKE_SOURCE_DIR}/shaders)
target_link_libraries(SDL_RESIZE_TEST SDL3::SDL3-shared Vulkan::Vulkan "-framework QuartzCore")
cmake_minimum_required(VERSION 3.16)
project(game)

set(CMAKE_C_STANDARD 23)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_compile_options(-Wall -g)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

include_directories(./include)

file(GLOB_RECURSE SOURCES
    "./src/*.c",
    "./include/*.h")

# Create your game executable target as usual
add_executable(game ${SOURCES})

# Link to the actual SDL3 library.
target_link_libraries(game PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
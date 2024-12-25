cmake_minimum_required (VERSION 3.11)

# if *nix, try and use clang
if(UNIX)
        find_program(CMAKE_C_COMPILER NAMES $ENV{CC} clang PATHS ENV PATH NO_DEFAULT_PATH)
        find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} clang++ PATHS ENV PATH NO_DEFAULT_PATH)
endif()

project ("engine" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add all subdirectories of src
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE INC_FILES "src/*.hpp")

# Add source files to the executable
add_executable(engine ${SRC_FILES} ${INC_FILES})

# Grab SDL & OpenGL
find_library(SDL_LIB SDL2)
find_library(GL_LIB GL)
find_library(MAFS m)

# Set the include directory for the executable
target_include_directories(engine PRIVATE "src")

# Get the current working branch
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest commit hash
execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

# wall/wextra/etc
if(MSVC)
  target_compile_options(engine PRIVATE /W4 /WX)
else()
  target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")

target_link_libraries(engine PRIVATE "${SDL_LIB}" "${GL_LIB}" "${MAFS}")

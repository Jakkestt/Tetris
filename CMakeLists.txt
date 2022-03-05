cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_C_STANDARD 11)

project(Tetris)

file(GLOB_RECURSE SOURCES src/*.c)
add_executable(Tetris ${SOURCES})

add_subdirectory(include/SDL2/)
add_subdirectory(include/SDL_mixer/)
add_subdirectory(include/SDL_ttf/)
find_package(OpenGL REQUIRED)

string(REPLACE "-O2" "-O0" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "-O2" "-O0" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} m SDL2 SDL2_ttf SDL2_mixer ${CMAKE_DL_LIBS})

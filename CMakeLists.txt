cmake_minimum_required(VERSION 3.0.2)
project(UTTT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-ggdb)

set(PROJECT_LINK_LIBS libsfml-audio.so libsfml-graphics.so libsfml-system.so libsfml-window.so libXrandr.so libXcursor.so)
link_directories(/usr/local/lib)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(UTTT ${SOURCES})
target_link_libraries(UTTT ${PROJECT_LINK_LIBS})
cmake_minimum_required(VERSION 3.8)
project(Luna)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB_RECURSE LUNA_SOURCES "Source/*.c" "Source/*.cpp")
file(GLOB_RECURSE LUNA_HEADERS "Source/*.h")

find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

add_executable(Luna ${LUNA_SOURCES} ${LUNA_HEADERS})
target_include_directories(Luna PRIVATE "Include")
target_link_libraries(Luna glfw glm dl pthread)

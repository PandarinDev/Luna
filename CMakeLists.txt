cmake_minimum_required(VERSION 3.8)
project(Luna)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
file(GLOB_RECURSE LUNA_SOURCES "Source/*.c" "Source/*.cpp")
file(GLOB_RECURSE LUNA_HEADERS "Source/*.h")

find_package(glfw3 CONFIG 3.3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(Luna ${LUNA_SOURCES} ${LUNA_HEADERS})
target_include_directories(Luna PRIVATE "Include")
target_link_libraries(Luna glfw glm ${CMAKE_DL_LIBS} Threads::Threads)
set_target_properties(Luna PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
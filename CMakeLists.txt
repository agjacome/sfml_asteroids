cmake_minimum_required(VERSION 2.8)
project(ASTEROIDS)

# compilation flags
set(CMAKE_CXX_FLAGS         "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")

# enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# define sources and executable
set(EXECUTABLE_NAME "asteroids")
set(ASTEROIDS_SOURCES
    sources/bullet.cpp
    sources/explosion.cpp
    sources/game.cpp
    sources/main.cpp
    sources/asteroid.cpp
    sources/ship.cpp)
include_directories(${ASTEROIDS_SOURCE_DIR}/include)
add_executable(${EXECUTABLE_NAME} ${ASTEROIDS_SOURCES})

# detect and add SFML
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

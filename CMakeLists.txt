cmake_minimum_required(VERSION 3.12)		# never set less 3.12 min version
project(btPlaylistCreator)

# Build type is debug by default
# call cmake .. -DCMAKE_BUILD_TYPE=Release to change to relase
# call cmake .. -DCMAKE_BUILD_TYPE=Debug to revert to debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 23)  # We're using C++23 standard :)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(ZLIB)
find_package(TagLib 2.0.0 REQUIRED)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include_directories(include)

# Choose wich executable will be build : cmake --build . -t <exe target> 

# main executable : cmake --build . -t bttManager
add_executable(btplcreator   src/btplcreator_main.cpp
                              src/lib_btplcreator.cpp
                              src/Playlist.cpp
                              )
target_link_libraries(btplcreator TagLib::tag)




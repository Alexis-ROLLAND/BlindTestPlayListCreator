cmake_minimum_required(VERSION 3.15)		# never set less 3.12 min version
project(btPlaylistCreator)

set(CMAKE_CXX_STANDARD 23)  # We're using C++23 standard :)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Désactiver les extensions spécifiques au compilateur
set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")


find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

# Build type is debug by default
# call cmake .. -DCMAKE_BUILD_TYPE=Release to change to relase
# call cmake .. -DCMAKE_BUILD_TYPE=Debug to revert to debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, etc.)" FORCE)
endif()

add_compile_options(-Wshadow -Wnon-virtual-dtor -Wold-style-cast)

# Définir des options de compilation pour tous les compilateurs
add_compile_options(-Wall -Wextra -Wpedantic)

# Ajout de flags spécifiques pour GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(SANITIZE_FLAGS "-fsanitize=address,undefined") # Choisissez vos sanitizers ici
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(${SANITIZE_FLAGS} -fno-omit-frame-pointer -g)
    add_link_options(${SANITIZE_FLAGS})
  endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")             # Debug : Optimisation légère pour le débogage
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")     # Release : Optimisation maximale sans symboles de debug

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package(ZLIB)
find_package(TagLib 2.0.0 REQUIRED)
find_package(wxWidgets 3.2 COMPONENTS core base REQUIRED CONFIG) # Request the required wxWidgets libs for GUI version

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Choose wich executable will be build : cmake --build . -t <exe target> 

# main executable : cmake --build . -t btplcreator
add_executable(btplcreator    src/btplcreator_main.cpp
                              src/lib_btplcreator.cpp
                              src/Playlist.cpp
                              )
# Inclure les répertoires d'en-têtes
target_include_directories(btplcreator PRIVATE include)
target_link_libraries(btplcreator TagLib::tag)


# GUI executable : cmake --build . -t btplcreatorGUI
add_executable(btplcreatorGUI     src/btplcreator_mainGUI.cpp
                                  src/mainframe.cpp
                                  src/Playlist.cpp
                                  )
# Inclure les répertoires d'en-têtes
target_include_directories(btplcreatorGUI PRIVATE include)
#target_link_libraries(btplcreatorGUI TagLib::tag)
target_link_libraries(btplcreatorGUI  wxWidgets::wxWidgets)






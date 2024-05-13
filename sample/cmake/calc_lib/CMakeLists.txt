cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(CALC VERSION 1.0)

# set specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set where the static and shared libraries are built
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# (optional) build static or shared libraries (-DBUILD_SHARED_LIBS=OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# generate config.h that define version
configure_file(config.h.in config.h)

# build deps CMAKE project
add_subdirectory(deps)

# compile executable calc
add_executable(calc main.cpp)

# add include directory when compile calc
target_include_directories(calc PUBLIC "${PROJECT_BINARY_DIR}")

# add library directory when compile calc
target_link_libraries(calc PUBLIC mymath)

# install calc to bin directory
install(TARGETS calc DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)

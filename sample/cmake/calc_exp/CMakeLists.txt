cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(CALC VERSION 1.0)

# set which flags to apply given a language and a set of compiler ids
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# define generator expressions (cmake >= 3.15)
add_library(calc_compiler_flags INTERFACE)

# set specify the C++ standard
target_compile_features(calc_compiler_flags INTERFACE cxx_std_11)

# add compiler warning flags just when building this project
target_compile_options(calc_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

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

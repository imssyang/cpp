cmake_minimum_required(VERSION 3.15)

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

if(APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

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

# install the configuration targets
install(EXPORT MyMathTargets
  FILE MyMathTargets.cmake
  DESTINATION lib/cmake/mymath
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/MyMathConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MyMathConfigVersion.cmake"
  VERSION "${CALC_VERSION_MAJOR}.${CALC_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the generated configuration files
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MyMathConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MyMathConfigVersion.cmake
  DESTINATION lib/cmake/mymath
  )

# generate the export targets for the build tree
export(EXPORT MyMathTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/MyMathTargets.cmake"
)

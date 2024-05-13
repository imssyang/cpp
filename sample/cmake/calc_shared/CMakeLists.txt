cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(CALC VERSION 1.0)

# set specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# (optional) use custom math functions (-DUSE_MYMATH=OFF)
option(USE_MYMATH "Use custom math implementation" ON)

# generate config.h that define version
configure_file(config.h.in config.h)

# add the mymath library
if(USE_MYMATH)
  # build deps
  add_subdirectory(deps)

  # store extra librarys to list
  list(APPEND EXTRA_LIBS mymath)

  # (optional) store extra include directories (no need when deps declare INTERFACE)
  #list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/deps")
endif()

# compile executable calc
add_executable(calc main.cpp)

# add include directory when compile calc
target_include_directories(calc PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})

# add library directory when compile calc
target_link_libraries(calc PUBLIC ${EXTRA_LIBS})

# install calc to bin directory
install(TARGETS calc DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)

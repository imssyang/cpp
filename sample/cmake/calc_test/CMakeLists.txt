cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(CALC VERSION 1.0)

# set specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# generate config.h that define version
configure_file(config.h.in config.h)

# compile executable calc
add_executable(calc main.cpp)

# add include directory when compile calc
target_include_directories(calc PUBLIC "${PROJECT_BINARY_DIR}")

# install calc to bin directory
install(TARGETS calc DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)

# enable testing
enable_testing()

# test-cast: calc 25
add_test(NAME Runs COMMAND calc 25)

# test-cast: check usage info
add_test(NAME Usage COMMAND calc)
set_tests_properties(Usage PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")

# define a function to simplify adding tests
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction()

# test-casts: calc by function
do_test(calc 4 "4 is 2")
do_test(calc 9 "9 is 3")
do_test(calc 5 "5 is 2.236")
do_test(calc 7 "7 is 2.645")
do_test(calc 25 "25 is 5")
do_test(calc -25 "-25 is (-nan|nan|0)")
do_test(calc 0.0001 "0.0001 is 0.01")

# compile to libmymath.so
add_library(mymath SHARED mysqrt.cxx)

# (optional) auto add include directory when compile item that depends mymath
target_include_directories(mymath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# (optional) check system provide the log and exp functions?
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::log(1.0);
    return 0;
  }
" HAVE_LOG)
check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::exp(1.0);
    return 0;
  }
" HAVE_EXP)

if(HAVE_LOG AND HAVE_EXP)
  # add compile definitions
  target_compile_definitions(mymath PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()

# install libmymath.a to lib directory
install(TARGETS mymath DESTINATION lib)
install(FILES mymath.h DESTINATION include)

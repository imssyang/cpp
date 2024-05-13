# compile libmymath.so
add_library(mymath mymath.cxx)

# (optional) auto add include directory when compile item that depends mymath
target_include_directories(mymath
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

# (optional) use custom math functions (-DUSE_MYMATH=OFF)
option(USE_MYMATH "Use custom math implementation" ON)

if(USE_MYMATH)
  # define USE_MYMATH when compile mymath
  target_compile_definitions(mymath PRIVATE "USE_MYMATH")

  # compile libmysqrt.a
  add_library(mysqrt STATIC
              mysqrt.cxx
              )

  # set mysqrt need PIC when the default is shared libraries
  set_target_properties(mysqrt PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

  # link mysqrt library when compile mymath
  target_link_libraries(mymath PRIVATE mysqrt)

  # link calc_compiler_flags when compile mymath
  target_link_libraries(mymath PRIVATE calc_compiler_flags)
endif()

# define EXPORTING_MYMATH when building on windows
target_compile_definitions(mymath PRIVATE "EXPORTING_MYMATH")

# install rules
set(installable_libs mymath calc_compiler_flags)
if(TARGET mysqrt)
  list(APPEND installable_libs mysqrt)
endif()
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES mymath.h DESTINATION include)

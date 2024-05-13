# compile make_table
add_executable(make_table make_table.cxx)

# generate table.h by make_table
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/table.h
  COMMAND make_table ${CMAKE_CURRENT_BINARY_DIR}/table.h
  DEPENDS make_table
  )

# compile to libmymath.so
add_library(mymath SHARED
  mysqrt.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/table.h
  )

# add include directory when compile mymath or item that depends mymath
target_include_directories(mymath
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
  )

# install libmymath.a to lib directory
install(TARGETS mymath DESTINATION lib)
install(FILES mymath.h DESTINATION include)

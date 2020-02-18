set(sources
  cinterop.f90
  saydx.f90
  )

add_library(saydx_fortran ${sources})
target_include_directories(saydx_fortran PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(saydx_fortran saydx)

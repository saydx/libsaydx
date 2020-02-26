set(sources
  array.f90
  ciface.f90
  cinterop.f90
  common.f90
  commontypes.f90
  eventhandler.f90
  eventprinter.f90
  msdparser.f90
  node.f90
  query.f90
  saydx.f90
  treebuilder.f90
  )

add_library(saydx_fortran ${sources})
target_include_directories(saydx_fortran PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(saydx_fortran saydx)

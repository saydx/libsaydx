set(sources
  array.c
  commontypes.c
  eventprinter.c
  msdparser.c
  node.c
  query.c
  saydx.c
  treebuilder.c
  treewalker.c)

add_library(saydx ${sources})
target_include_directories(saydx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

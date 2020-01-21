set(sources
  commontypes.c
  eventprinter.c
  msdparser.c
  node.c
  treebuilder.c
  treewalker.c)

add_library(saydx ${sources})
target_include_directories(saydx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

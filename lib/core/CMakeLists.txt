set(sources
  array.c
  commontypes.c
  eventprinter.c
  msdparser.c
  node.c
  query.c
  saydx.c
  serialize.c
  treebuilder.c
  treepacker.c
  treeunpacker.c
  treewalker.c
  txtdeserializer.c
  txtserializer.c)

add_library(saydx ${sources})
target_include_directories(saydx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

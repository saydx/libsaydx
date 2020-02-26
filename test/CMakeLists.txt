add_executable(test1 test1.c)
target_link_libraries(test1 saydx)

add_executable(test1f test1f.f90)
target_link_libraries(test1f saydx_fortran)

add_executable(test2 test2.c)
target_link_libraries(test2 saydx)

add_executable(test2f test2f.f90)
target_link_libraries(test2f saydx_fortran)

add_executable(test3 test3.c)
target_link_libraries(test3 saydx)

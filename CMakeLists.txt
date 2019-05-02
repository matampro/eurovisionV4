cmake_minimum_required(VERSION 3.13)
project(eurovisionV4 C)

set(CMAKE_C_STANDARD 99)

add_executable(eurovisionV4 eurovision.c map.h map.c list.h eurovisionTestsMain.c eurovisionTests.h eurovisionTests.c test_utilities.h)

target_link_libraries(eurovisionV4  ${CMAKE_CURRENT_SOURCE_DIR}/libmtm.a)
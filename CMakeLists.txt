
cmake_minimum_required (VERSION 2.6)
project (nekolib)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE NEKOLIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(nekolib ${NEKOLIB_TYPE} ${NEKOLIB_SOURCES})

if (UNIX AND NOT APPLE)
  target_link_libraries(nekolib stdc++fs rt pthread)
elseif (WIN32)
  target_link_libraries(nekolib Ws2_32)
endif ()
target_include_directories(nekolib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

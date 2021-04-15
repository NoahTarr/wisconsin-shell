cmake_minimum_required(VERSION 3.0.0)
project(wish VERSION 0.1.0)

include(CTest)
enable_testing()

# add_executable(batchCommand batchCommand.cpp)
set(SOURCES wish.cpp batchCommand.cpp batchCommand.h)
add_executable(wish ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

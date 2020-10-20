cmake_minimum_required(VERSION 3.0.0)
project(continuous-dataframe-stream VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_subdirectory(src)
add_subdirectory(examples)

include(CTest)
enable_testing()

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

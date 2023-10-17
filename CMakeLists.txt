cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(cmdline)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${PROJECT_IS_TOP_LEVEL})
  add_subdirectory(examples)
endif()
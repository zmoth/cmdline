cmake_minimum_required(VERSION 3.15)

project(cmdline)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${PROJECT_IS_TOP_LEVEL})
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

  add_subdirectory(examples)
endif()
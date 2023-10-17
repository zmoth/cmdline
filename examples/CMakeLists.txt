add_executable(test1 test.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(test1 PRIVATE /utf-8)
endif()

add_executable(test2 test2.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(test2 PRIVATE /utf-8)
endif()
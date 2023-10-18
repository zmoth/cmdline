add_executable(shell main.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(shell PRIVATE /utf-8)
endif()
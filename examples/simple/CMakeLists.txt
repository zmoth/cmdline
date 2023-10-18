add_executable(simple main.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(simple PRIVATE /utf-8)
endif()

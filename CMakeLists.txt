cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_STANDARD 17)

#set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(spekter CXX)
enable_testing()

file(GLOB headers include/spekter.h)

file(GLOB sources src/main.cpp src/tokenizer/token/token.cpp src/tokenizer/tokenizer.cpp src/tokenizer/file_tokenizer.cpp)

file(GLOB tests test/test.cc)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

add_executable(spekter ${sources} ${headers})
add_executable(run_tests ${tests})

add_subdirectory(test)

enable_testing()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(
  run_tests 
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)

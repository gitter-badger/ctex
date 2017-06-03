cmake_minimum_required(VERSION 3.0)
project(CTex)

# main target
include_directories(main/include)
file(GLOB_RECURSE sources main/src/*.cpp main/include/*.hpp)
add_executable(ctex ${sources})
target_compile_options(ctex PUBLIC -std=c++11)
target_include_directories(ctex PUBLIC main/src)

# testing 
include_directories(test/include)
file(GLOB_RECURSE sources_test test/src/*.cpp test/include/*.hpp)
list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/main/src/main.cpp")

add_executable(catch_tests ${sources_test} ${sources})
target_compile_definitions(catch_tests PUBLIC CATCH_TESTS)
set_target_properties(ctex PROPERTIES ENABLE_EXPORTS on)
target_link_libraries(catch_tests PUBLIC
    ctex
)

# Instal
install(TARGETS ctex DESTINATION bin)
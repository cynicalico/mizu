cmake_minimum_required(VERSION 4.0)
project(mizu)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(mizu INTERFACE)

add_subdirectory(thirdparty)
target_link_libraries(mizu INTERFACE mizu_thirdparty)

add_subdirectory(examples)

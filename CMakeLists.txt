cmake_minimum_required(VERSION 3.30)
project(mizu LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

set(${PROJECT_NAME}_headers
        include/mizu/engine.hpp
        include/mizu/log.hpp
        include/mizu/types.hpp
        include/mizu/window.hpp
)

set(${PROJECT_NAME}_sources
        src/mizu/engine.cpp
        src/mizu/window.cpp
)

add_library(mizu ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(thirdparty)
target_link_libraries(${PROJECT_NAME} PUBLIC mizu_thirdparty)

add_subdirectory(examples)

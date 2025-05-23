cmake_minimum_required(VERSION 3.30)
project(mizu LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

set(${PROJECT_NAME}_headers
        #[=[ [[[cog
        import cog, glob
        from pathlib import Path
        patterns = ['include/**/*.h', 'include/**/*.hpp']
        for p in patterns:
            for f in glob.glob(p, recursive=True):
                cog.outl(f"\t{Path(f).as_posix()}")
        ]]] ]=]
        include/gloo/context.hpp
        include/gloo/shader.hpp
        include/gloo/sdl3/gl_attr.hpp
        include/gloo/sdl3/gl_context_flags.hpp
        include/mizu/application.hpp
        include/mizu/callback_mgr.hpp
        include/mizu/class_helpers.hpp
        include/mizu/color.hpp
        include/mizu/engine.hpp
        include/mizu/enum_class_helpers.hpp
        include/mizu/g2d.hpp
        include/mizu/image.hpp
        include/mizu/input_mgr.hpp
        include/mizu/log.hpp
        include/mizu/mizu.hpp
        include/mizu/payloads.hpp
        include/mizu/platform.hpp
        include/mizu/types.hpp
        include/mizu/window.hpp
        include/mizu/ds/priority_queue.hpp
        # [[[end]]]
)

set(${PROJECT_NAME}_sources
        #[=[ [[[cog
        import cog, glob
        from pathlib import Path
        patterns = ['src/**/*.c', 'src/**/*.cpp']
        for p in patterns:
            for f in glob.glob(p, recursive=True):
                cog.outl(f"\t{Path(f).as_posix()}")
        ]]] ]=]
        src/gloo/context.cpp
        src/gloo/shader.cpp
        src/gloo/sdl3/gl_attr.cpp
        src/gloo/sdl3/gl_context_flags.cpp
        src/mizu/application.cpp
        src/mizu/color.cpp
        src/mizu/engine.cpp
        src/mizu/g2d.cpp
        src/mizu/image.cpp
        src/mizu/input_mgr.cpp
        src/mizu/platform.cpp
        src/mizu/window.cpp
        # [[[end]]]
)

add_library(mizu ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC include)

if (WIN32)
    target_compile_definitions(mizu PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif ()

if (MIZU_SPDLOG_LEVEL)
    target_compile_definitions(mizu PRIVATE SPDLOG_ACTIVE_LEVEL=${MIZU_SPDLOG_LEVEL})
endif ()

add_subdirectory(thirdparty)
target_link_libraries(${PROJECT_NAME} PUBLIC mizu_thirdparty)

add_subdirectory(examples)

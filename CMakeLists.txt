cmake_minimum_required(VERSION 3.30)
project(mizu LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(MIZU_BUILD_EXAMPLE "Build example programs" OFF)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

set(mizu_headers
        #[=[ [[[cog
        import cog, glob
        from pathlib import Path
        patterns = ['include/**/*.h', 'include/**/*.hpp']
        for p in patterns:
            for f in glob.glob(p, recursive=True):
                cog.outl(f"\t{Path(f).as_posix()}")
        ]]] ]=]
        include/gloo/buffer.hpp
        include/gloo/context.hpp
        include/gloo/shader.hpp
        include/gloo/texture.hpp
        include/gloo/vertex_array.hpp
        include/gloo/sdl3/attr.hpp
        include/gloo/sdl3/context_flags.hpp
        include/mizu/imconfig.hpp
        include/mizu/mizu.hpp
        include/mizu/core/application.hpp
        include/mizu/core/audio_mgr.hpp
        include/mizu/core/batcher.hpp
        include/mizu/core/callback_mgr.hpp
        include/mizu/core/color.hpp
        include/mizu/core/dear.hpp
        include/mizu/core/engine.hpp
        include/mizu/core/g2d.hpp
        include/mizu/core/input_mgr.hpp
        include/mizu/core/log.hpp
        include/mizu/core/payloads.hpp
        include/mizu/core/texture.hpp
        include/mizu/core/window.hpp
        include/mizu/ds/priority_queue.hpp
        include/mizu/font/code_page_437.hpp
        include/mizu/util/averagers.hpp
        include/mizu/util/class_helpers.hpp
        include/mizu/util/enum_class_helpers.hpp
        include/mizu/util/io.hpp
        include/mizu/util/is_any_of.hpp
        include/mizu/util/memusage.hpp
        include/mizu/util/platform.hpp
        include/mizu/util/rng.hpp
        include/mizu/util/shapes.hpp
        include/mizu/util/time.hpp
        # [[[end]]]
)

set(mizu_sources
        #[=[ [[[cog
        import cog, glob
        from pathlib import Path
        patterns = ['src/**/*.c', 'src/**/*.cpp']
        for p in patterns:
            for f in glob.glob(p, recursive=True):
                cog.outl(f"\t{Path(f).as_posix()}")
        ]]] ]=]
        src/gloo/buffer.cpp
        src/gloo/context.cpp
        src/gloo/shader.cpp
        src/gloo/texture.cpp
        src/gloo/vertex_array.cpp
        src/gloo/sdl3/attr.cpp
        src/gloo/sdl3/context_flags.cpp
        src/mizu/core/application.cpp
        src/mizu/core/audio_mgr.cpp
        src/mizu/core/batcher.cpp
        src/mizu/core/color.cpp
        src/mizu/core/dear.cpp
        src/mizu/core/engine.cpp
        src/mizu/core/g2d.cpp
        src/mizu/core/input_mgr.cpp
        src/mizu/core/texture.cpp
        src/mizu/core/window.cpp
        src/mizu/font/code_page_437.cpp
        src/mizu/util/averagers.cpp
        src/mizu/util/io.cpp
        src/mizu/util/memusage.cpp
        src/mizu/util/platform.cpp
        src/mizu/util/rng.cpp
        # [[[end]]]
)

add_library(mizu ${mizu_headers} ${mizu_sources})

target_compile_features(mizu PUBLIC cxx_std_23)

target_include_directories(mizu PUBLIC include)

if (WIN32)
    target_compile_definitions(mizu PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
    target_compile_options(mizu PUBLIC /utf-8 /W4)
endif ()

target_compile_definitions(mizu PUBLIC
        IMGUI_USER_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/include/mizu/imconfig.hpp"
)

add_subdirectory(thirdparty)
target_link_libraries(mizu PUBLIC mizu_thirdparty)

add_library(mizu::mizu ALIAS mizu)

if (MIZU_BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()

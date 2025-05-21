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
        include/mizu/log.hpp
        include/mizu/application.hpp
        include/mizu/callback_mgr.hpp
        include/mizu/color.hpp
        include/mizu/engine.hpp
        include/mizu/enum_class_helpers.hpp
        include/mizu/g2d.hpp
        include/mizu/image.hpp
        include/mizu/input_mgr.hpp
        include/mizu/mizu.hpp
        include/mizu/payloads.hpp
        include/mizu/types.hpp
        include/mizu/window.hpp
        include/mizu/platform.hpp
        include/mizu/ds/priority_queue.hpp
        include/gloo/context.hpp
        include/gloo/sdl3/gl_attr.hpp
        include/gloo/sdl3/gl_context_flags.hpp
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
        src/mizu/application.cpp
        src/mizu/color.cpp
        src/mizu/engine.cpp
        src/mizu/g2d.cpp
        src/mizu/image.cpp
        src/mizu/input_mgr.cpp
        src/mizu/window.cpp
        src/mizu/platform.cpp
        src/gloo/context.cpp
        src/gloo/sdl3/gl_attr.cpp
        src/gloo/sdl3/gl_context_flags.cpp
        # [[[end]]]
)

add_library(mizu ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(thirdparty)
target_link_libraries(${PROJECT_NAME} PUBLIC mizu_thirdparty)

add_subdirectory(examples)

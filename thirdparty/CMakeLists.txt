include(get_cpm.cmake)

CPMAddPackage(
        NAME sdl3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-3.2.10
        OPTIONS "BUILD_SHARED_LIBS OFF")

# We need Jinja2 for Glad 2 to work
find_package(Python3 REQUIRED COMPONENTS Interpreter)
execute_process(COMMAND ${Python3_EXECUTABLE} -m pip show Jinja2 RESULT_VARIABLE EXIT_CODE OUTPUT_QUIET)
if (NOT ${EXIT_CODE} EQUAL 0)
    message(FATAL_ERROR "The Jinja2 Python package is not installed. Please install it using the following command: \"${Python3_EXECUTABLE} -m pip install jinja2\".")
endif ()

CPMAddPackage(
        NAME glad2
        GITHUB_REPOSITORY Dav1dde/glad
        GIT_TAG 56e406e
        DOWNLOAD_ONLY YES
)

add_subdirectory(${glad2_SOURCE_DIR}/cmake ${glad2_BINARY_DIR})
if (WIN32)
    glad_add_library(glad_gl_core_mx_43 STATIC REPRODUCIBLE MX API gl:core=4.3 wgl=1.0)
else ()
    glad_add_library(glad_gl_core_mx_43 STATIC REPRODUCIBLE MX API gl:core=4.3 egl=1.5)
endif ()

CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 2d4c4b4
        OPTIONS "BUILD_SHARED_LIBS OFF" "GLM_ENABLE_CXX_20 ON" "GLM_ENABLE_LANG_EXTENSIONS ON"
)

set(OPENAL_SOFT_CPM_OPTIONS "ALSOFT_UTILS OFF" "ALSOFT_EXAMPLES OFF")
if (MSVC)
    set(OPENAL_SOFT_CPM_OPTIONS ${OPENAL_SOFT_CPM_OPTIONS}
            "ALSOFT_BACKEND_WINMM OFF"
            "ALSOFT_REQUIRE_WASAPI ON"
            "ALSOFT_BACKEND_DSOUND OFF"
            "ALSOFT_BACKEND_WAVE OFF"
    )
else ()
    set(OPENAL_SOFT_CPM_OPTIONS ${OPENAL_SOFT_CPM_OPTIONS}
            "ALSOFT_BACKEND_PIPEWIRE ON"
            "ALSOFT_REQUIRE_PIPEWIRE ON"
            "ALSOFT_BACKEND_PULSEAUDIO OFF"
            "ALSOFT_BACKEND_OSS OFF"
            "ALSOFT_BACKEND_SOLARIS OFF"
    )
endif ()
CPMAddPackage(
        NAME openal-soft
        GITHUB_REPOSITORY kcat/openal-soft
        GIT_TAG 481c9e0
        OPTIONS ${OPENAL_SOFT_CPM_OPTIONS}
)

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 11.1.4
        OPTIONS "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.2
        OPTIONS "BUILD_SHARED_LIBS OFF" "SPDLOG_FMT_EXTERNAL ON"
)

CPMAddPackage(
        NAME libsndfile
        GITHUB_REPOSITORY libsndfile/libsndfile
        GIT_TAG ea9ff56
        OPTIONS "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME pcg
        GITHUB_REPOSITORY imneme/pcg-cpp
        GIT_TAG 428802d
        DOWNLOAD_ONLY YES
)

CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG f4d9359
        DOWNLOAD_ONLY TRUE
)

find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)

add_library(mizu_thirdparty STATIC
        ${pcg_SOURCE_DIR}/include/pcg_extras.hpp
        ${pcg_SOURCE_DIR}/include/pcg_random.hpp
        ${pcg_SOURCE_DIR}/include/pcg_uint128.hpp

        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
        ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.h

        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
)

target_compile_definitions(mizu_thirdparty PUBLIC
        IMGUI_USER_CONFIG="${CMAKE_SOURCE_DIR}/include/mizu/imconfig.hpp"
)

target_include_directories(mizu_thirdparty PUBLIC
        ${pcg_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(mizu_thirdparty PUBLIC
        fmt::fmt
        spdlog::spdlog
        glm::glm
        glad_gl_core_mx_43
        SndFile::sndfile
        OpenAL::OpenAL
        SDL3::SDL3
        PNG::PNG
        Freetype::Freetype
)

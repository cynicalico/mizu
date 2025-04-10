include(get_cpm.cmake)

CPMAddPackage(
        NAME sdl3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-3.2.10
        OPTIONS "BUILD_SHARED_LIBS OFF")

CPMAddPackage(
        NAME sdl3_mixer
        GITHUB_REPOSITORY libsdl-org/SDL_mixer
        GIT_TAG a983517
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
        DOWNLOAD_ONLY YES)

add_subdirectory(${glad2_SOURCE_DIR}/cmake ${glad2_BINARY_DIR})
if (WIN32)
    glad_add_library(glad_gl_core_mx_33 STATIC REPRODUCIBLE MX API gl:core=3.3 wgl=1.0)
else ()
    glad_add_library(glad_gl_core_mx_33 STATIC REPRODUCIBLE MX API gl:core=3.3 egl=1.5)
endif ()

CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 2d4c4b4
        OPTIONS "BUILD_SHARED_LIBS OFF" "GLM_ENABLE_CXX_20 ON" "GLM_ENABLE_LANG_EXTENSIONS ON")

if (MSVC)
    set(OPENAL_SOFT_CPM_OPTIONS "ALSOFT_UTILS OFF" "ALSOFT_EXAMPLES OFF" "ALSOFT_REQUIRE_WASAPI ON")
else ()
    set(OPENAL_SOFT_CPM_OPTIONS "ALSOFT_UTILS OFF" "ALSOFT_EXAMPLES OFF")
endif ()
CPMAddPackage(
        NAME openal-soft
        GITHUB_REPOSITORY kcat/openal-soft
        GIT_TAG 481c9e0
        OPTIONS ${OPENAL_SOFT_CPM_OPTIONS})

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 11.1.4
        OPTIONS "BUILD_SHARED_LIBS OFF")

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.2
        OPTIONS "BUILD_SHARED_LIBS OFF" "SPDLOG_FMT_EXTERNAL ON")

add_library(mizu_thirdparty INTERFACE)

target_link_libraries(mizu_thirdparty INTERFACE
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        glad_gl_core_mx_33
        glm::glm
        OpenAL::OpenAL
        fmt::fmt
        spdlog::spdlog)

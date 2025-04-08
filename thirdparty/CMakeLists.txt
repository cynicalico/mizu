include(get_cpm.cmake)

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
    glad_add_library(glad_gl_core_mx_33 REPRODUCIBLE MX API gl:core=3.3 wgl=1.0)
else ()
    glad_add_library(glad_gl_core_mx_33 REPRODUCIBLE MX API gl:core=3.3 egl=1.5)
endif ()

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 11.1.4)

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.2
        OPTIONS "SPDLOG_FMT_EXTERNAL ON")

CPMAddPackage(
        NAME sdl3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-3.2.10)

CPMAddPackage(
        NAME libpng
        GITHUB_REPOSITORY pnggroup/libpng
        VERSION 1.6.47
        OPTIONS "PNG_STATIC OFF" "PNG_TESTS OFF" "PNG_TOOLS OFF")

CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 2d4c4b4
        OPTIONS "BUILD_SHARED_LIBS OFF" "GLM_ENABLE_CXX_20 ON" "GLM_ENABLE_LANG_EXTENSIONS ON")

CPMAddPackage(
        NAME sndfile
        GITHUB_REPOSITORY libsndfile/libsndfile
        GIT_TAG ea9ff56
        OPTIONS "BUILD_PROGRAMS OFF" "BUILD_EXAMPLES OFF")

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

add_library(mizu_thirdparty INTERFACE)

target_link_libraries(mizu_thirdparty INTERFACE
        fmt::fmt
        spdlog::spdlog
        glad_gl_core_mx_33
        SDL3::SDL3
        png_shared
        glm::glm
        OpenAL::OpenAL
        SndFile::sndfile)

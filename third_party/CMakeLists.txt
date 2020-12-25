#set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/third_party/_deps" CACHE STRING "" FORCE)
set(FETCHCONTENT_QUIET off CACHE BOOL "" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED on CACHE BOOL "" FORCE)

## Note the difference between FetchContent `https://cmake.org/cmake/help/latest/module/FetchContent.html`
## and ExternalProject `https://cmake.org/cmake/help/latest/module/ExternalProject.html`
## is the following
##
# - FetchContent is only perfomed in **configure** steps, and it is not rerun at each build step
# - ExternalProject is performed at every **build** step.
#
## Note that if the external third party content is touched or modified, it will
## still be rebuilt regardless

set(THIRD_PARTY_BASE_DIR
    "${CMAKE_SOURCE_DIR}/third_party/sources"
    CACHE STRING
    "Where third party sources should be downloaded and located")

include(FetchContent)
find_package(Git REQUIRED)


# Define custom Fetch function for submodules. If classical CMAKE FetchContent_Declare is used
# to download git submodules, it will without prompting just delete the submodule
# directory and start a new download from fresh each time Cmake configuration is perfomed.
# This behaviour is unwanted since, somebody may have modified the source
# code of a third party for testing reasons and cmake will happily delete it.
# This is especially true for people which have a workflow/habit of deleting the
# Cmake build directory. If the build directory is deleted, or a new build target
# is created (eg Release, Debug, RelWithDebugInfo)... Cmake will just delete the
# submodule directory contents and start a new clone... Very bad.
function(FetchGitSubmodule path)
    get_filename_component(name "${path}" NAME)

    FetchContent_Declare(
        ${name}
        DOWNLOAD_COMMAND ${GIT_EXECUTABLE} submodule update --recursive --init --depth 1 "${path}"
        SOURCE_DIR "${THIRD_PARTY_BASE_DIR}/${name}"
    )
    FetchContent_MakeAvailable("${name}")
endfunction()

FetchGitSubmodule("${THIRD_PARTY_BASE_DIR}/stb")
FetchGitSubmodule("${THIRD_PARTY_BASE_DIR}/Unity")

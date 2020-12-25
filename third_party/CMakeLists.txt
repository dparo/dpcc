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


find_package(Git REQUIRED)

function(FetchGitSubmodule GIT_REPOSITORY repo)
    get_filename_component(name "${repo}" NAME)

    FetchContent_Declare(
        ${name}
        DOWNLOAD_COMMAND ${GIT_EXECUTABLE} submodule update --init --depth 1 "${THIRD_PARTY_BASE_DIR}/${name}"
        SOURCE_DIR "${THIRD_PARTY_BASE_DIR}/${name}"
    )
endfunction()


include(FetchContent)

FetchGitSubmodule(GIT_REPOSITORY https://github.com/nothings/stb)
FetchGitSubmodule(GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity)

FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(Unity)

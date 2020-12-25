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

FetchContent_Declare(
    stb
    SOURCE_DIR "${THIRD_PARTY_BASE_DIR}/stb"
    GIT_REPOSITORY https://github.com/nothings/stb
    GIT_TAG b42009b3b9d4ca35bc703f5310eedc74f584be58
)

FetchContent_Declare(
    Unity
    SOURCE_DIR "${THIRD_PARTY_BASE_DIR}/Unity"
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
    GIT_TAG 386c540510929c0cf6f7b8d04cb4af40754bb822
)

FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(Unity)

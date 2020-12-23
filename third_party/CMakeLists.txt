include(FetchContent)


FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  PATCH_COMMAND ""
  GIT_TAG b42009b3b9d4ca35bc703f5310eedc74f584be58
)

FetchContent_Declare(Unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  PATCH_COMMAND ""
  GIT_TAG 386c540510929c0cf6f7b8d04cb4af40754bb822
)

FetchContent_MakeAvailable(stb Unity)

set(THIRD_PARTY_BASE_DIR "${FETCHCONTENT_BASE_DIR}" PARENT_SCOPE)

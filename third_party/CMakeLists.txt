include(FetchContent)


FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb
  PATCH_COMMAND ""
  GIT_TAG b42009b3b9d4ca35bc703f5310eedc74f584be58
)

FetchContent_Declare(Unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
  PATCH_COMMAND ""
  GIT_TAG 386c540510929c0cf6f7b8d04cb4af40754bb822
)

message(STATUS "Fetching stb...")
FetchContent_MakeAvailable(stb)
message(STATUS "Done!")
message(STATUS "Fetching unity...")
FetchContent_MakeAvailable(Unity)
message(STATUS "Done!")

set(THIRD_PARTY_BASE_DIR "${FETCHCONTENT_BASE_DIR}" PARENT_SCOPE)

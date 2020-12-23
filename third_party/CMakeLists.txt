include(FetchContent)
set(THIRD_PARTY_BASE_DIR "${FETCHCONTENT_BASE_DIR}" PARENT_SCOPE)

FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG b42009b3b9d4ca35bc703f5310eedc74f584be58
)

FetchContent_Declare(Unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
  GIT_TAG 386c540510929c0cf6f7b8d04cb4af40754bb822
)

message(STATUS "Fetching https://github.com/nothings/stb")
FetchContent_MakeAvailable(stb)
message(STATUS "Done!")
message(STATUS "Fetching https://github.com/ThrowTheSwitch/Unity")
FetchContent_MakeAvailable(Unity)
message(STATUS "Done!")

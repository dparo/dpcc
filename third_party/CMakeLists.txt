include(FetchContent)


FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  #SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/stb"
  PATCH_COMMAND ""
  GIT_TAG origin/master
)

FetchContent_Declare(Unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  #SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/Unity"
  PATCH_COMMAND ""
  GIT_TAG origin/master
)

FetchContent_MakeAvailable(stb Unity)

set(THIRD_PARTY_BASE_DIR "${FETCHCONTENT_BASE_DIR}" PARENT_SCOPE)

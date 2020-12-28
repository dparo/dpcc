cmake_minimum_required(VERSION 3.11)

project(dpcc)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/bin")

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()



if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wno-unused-function)
endif()


# Code Coverage Configuration
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

add_subdirectory(third_party)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/.local;${CMAKE_SOURCE_DIR}/.local/third_party;${CMAKE_PREFIX_PATH}")
find_package(BISON 3.6 REQUIRED)
find_package(FLEX 2.6 REQUIRED)


set(BISON_FLAGS "")

if (LINUX)
  set(BISON_FLAGS "--no-lines -fcaret -ffixit -Wdangling-alias -Wdeprecated -Wempty-rule -Wmidrule-values -Wother")
endif()


BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
  COMPILE_FLAGS "${BISON_FLAGS}"
  )

FLEX_TARGET(lexer  src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)


add_library(libstb STATIC
  third_party/stb_impl.c
  )
target_include_directories(libstb PUBLIC "${THIRD_PARTY_BASE_DIR}/submodules/stb")

add_library(libdpcc STATIC
  src/dpcc.c src/dpcc.h src/globals.c src/utils.c src/log.c src/yacc_utils.c src/codegen.c
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS}
  )
target_include_directories(libdpcc PUBLIC src)
target_include_directories(libdpcc PUBLIC generated_srcs/src)
target_include_directories(libdpcc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(libdpcc PUBLIC libstb)


add_subdirectory("${CMAKE_SOURCE_DIR}/generated_srcs")


add_executable(dpcc src/main.c)
target_link_libraries(dpcc libdpcc)

include(CTest)

if(BUILD_TESTING)
  target_compile_definitions(unity PRIVATE UNITY_INCLUDE_PRINT_FORMATTED UNITY_OUTPUT_COLOR)
  add_subdirectory(tests)
endif()

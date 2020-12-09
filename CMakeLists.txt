cmake_minimum_required(VERSION 3.2)

project(dpcc)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/bin")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)



if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()


if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wno-unused-function)
endif()


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
target_include_directories(libstb PUBLIC third_party/stb)

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


add_executable(dpcc src/main.c )
target_link_libraries(dpcc libdpcc)

# Testing goes at the end boys
include(CTest)

if(BUILD_TESTING)
  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/Unity")
  target_compile_definitions(unity PRIVATE UNITY_INCLUDE_PRINT_FORMATTED UNITY_OUTPUT_COLOR)
  add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.2)

project(dpcc)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree/bin")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wno-unused-function)
endif()


BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(lexer  src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)


add_library(libdpcc STATIC
  src/dpcc.c src/dpcc.h src/globals.c src/utils.c
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS}
  )
target_include_directories(libdpcc PUBLIC src)
target_include_directories(libdpcc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_executable(dpcc src/main.c )
target_link_libraries(dpcc libdpcc)

# Testing goes at the end boys
include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

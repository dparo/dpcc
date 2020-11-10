cmake_minimum_required(VERSION 3.2)

project(dpcc)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(scanner  src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_executable(dpcc
  src/main.c
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)


# Testing goes at the end boys
include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

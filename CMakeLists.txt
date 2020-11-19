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


BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
  COMPILE_FLAGS "-Wcounterexamples -Wempty-rule -Wmidrule-values --color=auto"
  )

FLEX_TARGET(lexer  src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)


add_library(libstb STATIC
  third_party/stb_impl.c
  )
target_include_directories(libstb PUBLIC third_party/stb)

add_library(libdpcc STATIC
  src/dpcc.c src/dpcc.h src/globals.c src/utils.c src/log.c src/yacc_utils.c
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS}
  )
target_include_directories(libdpcc PUBLIC src)
target_include_directories(libdpcc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(libdpcc PUBLIC libstb)




add_custom_target(gen.py
  python3 "${CMAKE_SOURCE_DIR}/generated_srcs/gen_all.py"
  DEPENDS "${CMAKE_SOURCE_DIR}/generated_srcs/gen_all.py"
  BYPRODUCTS "${CMAKE_SOURCE_DIR}/generated_srcs/last_gen.txt"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/generated_srcs"
  )

add_dependencies(libdpcc gen.py)
target_sources(libdpcc PRIVATE  "${CMAKE_SOURCE_DIR}/generated_srcs/src/gen.c")


add_executable(dpcc src/main.c )
target_link_libraries(dpcc libdpcc)

# Testing goes at the end boys
include(CTest)

if(BUILD_TESTING)
  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/Unity")
  add_subdirectory(tests)
endif()

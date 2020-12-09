set(TEST_SOURCES_LIST
  "lexer_test.c"
  "parser_test.c"
  )

foreach(SRC IN LISTS TEST_SOURCES_LIST)
  get_filename_component(TARGET "${SRC}" NAME_WE)

  add_executable("${TARGET}" "${SRC}")
  set_target_properties("${TARGET}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")
  target_include_directories("${TARGET}" PRIVATE ./ )

  target_link_libraries("${TARGET}" PRIVATE unity libdpcc)
  target_link_libraries("${TARGET}" PRIVATE coverage_config)
  target_compile_definitions("${TARGET}" PRIVATE UNITY_INCLUDE_PRINT_FORMATTED)
  target_compile_definitions("${TARGET}" PRIVATE _GNU_SOURCE UNITY_OUTPUT_COLOR)

  add_test(NAME "${TARGET}" COMMAND "${TARGET}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree"
    )
endforeach(SRC)


add_test(NAME compile_test.py
  COMMAND python3 "${CMAKE_SOURCE_DIR}/tests/compile_test.py"
  DEPENDS "${CMAKE_SOURCE_DIR}/tests/compile_test.py"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

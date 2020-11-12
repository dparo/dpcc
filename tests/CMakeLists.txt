set(TEST_SOURCES_LIST
  "scanner_test.c"
  "parser_test.c"
  )

find_package(CMocka REQUIRED)

foreach(V IN LISTS TEST_SOURCES_LIST)
  get_filename_component(name "${V}" NAME_WE)

  add_executable("${name}" "${V}")
  target_include_directories("${name}" PRIVATE ${CMOCKA_INCLUDE_DIRS})
  target_link_libraries("${name}" PRIVATE ${CMOCKA_LIBRARIES})
  target_link_libraries("${name}" PRIVATE libdpcc)
  set_target_properties("${name}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")

  add_test(NAME "${name}" COMMAND "${name}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree"
    )
endforeach(V)

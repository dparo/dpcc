set(TEST_SOURCES_LIST
  "scanner_test.c"

  )

find_package(CMocka REQUIRED)

foreach(V IN LISTS TEST_SOURCES_LIST)
  get_filename_component(name "${V}" NAME_WE)
  add_executable("${name}" "${V}")
  target_include_directories("${name}" PRIVATE ${CMOCKA_INCLUDE_DIRS})
  target_link_libraries("${name}" PRIVATE ${CMOCKA_LIBRARIES})
  add_test(NAME "${name}" COMMAND "${V}")
endforeach(V)

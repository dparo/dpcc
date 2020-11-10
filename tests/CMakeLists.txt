set(TEST_SOURCES_LIST
  "scanner_test.c"

  )

foreach(V IN LISTS TEST_SOURCES_LIST)
  get_filename_component(name "${V}" NAME_WE)
  add_executable("${name}" "${V}")
  add_test(NAME "${name}" COMMAND "${V}")
endforeach(V)

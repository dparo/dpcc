set(TEST_SOURCES_LIST "scanner_test")


foreach(V IN LISTS TEST_SOURCES_LIST)
  add_executable("${V}" "${V}.c")
  add_test(NAME "${V}" COMMAND "${V}")
endforeach(V)

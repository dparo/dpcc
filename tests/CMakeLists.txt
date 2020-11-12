set(TEST_SOURCES_LIST
  "scanner_test.c"
  "parser_test.c"
  )

find_package(CMocka)

# No need to declare and build the tests if the library is not found.
# Do not impose extra dependecy on the user for no reason.
if (NOT ${CMOCKA_LIBRARIES})
  foreach(SRC IN LISTS TEST_SOURCES_LIST)
    get_filename_component(TARGET "${SRC}" NAME_WE)

    add_executable("${TARGET}" "${SRC}")
    target_include_directories("${TARGET}" PRIVATE ${CMOCKA_INCLUDE_DIRS})
    target_link_libraries("${TARGET}" PRIVATE ${CMOCKA_LIBRARIES})
    target_link_libraries("${TARGET}" PRIVATE libdpcc)
    set_target_properties("${TARGET}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")

    add_test(NAME "${TARGET}" COMMAND "${TARGET}"
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree"
      )
  endforeach(SRC)
endif()

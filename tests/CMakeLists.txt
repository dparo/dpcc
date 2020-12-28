set(TEST_SOURCES_LIST
    "lexer_test.c"
    "parser_test.c"
 )

# Disable these tests under MACOS since for some reason
# the build fails with the following message
#   ```
#   Undefined symbols for architecture x86_64:
#    "_yylloc", referenced from:
#        _clear_all_global_vars in liblibdpcc.a(globals.c.o)
#    "_yynerrs", referenced from:
#        _clear_all_global_vars in liblibdpcc.a(globals.c.o)
#   ```
# This doesn't make any sense since we are able to link libdpcc without
# any problem in the dpcc executable... I can't understand
# where the problem comes from and i don't have a MAC os machine
# at my disposal

if (UNIX AND NOT APPLE)
    foreach(SRC IN LISTS TEST_SOURCES_LIST)
        get_filename_component(TARGET "${SRC}" NAME_WE)

        add_executable("${TARGET}" "${SRC}")
        set_target_properties("${TARGET}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")
        target_include_directories("${TARGET}" PRIVATE ./ )

        target_compile_definitions("${TARGET}" PRIVATE UNITY_INCLUDE_PRINT_FORMATTED)
        target_compile_definitions("${TARGET}" PRIVATE UNITY_OUTPUT_COLOR)

        target_link_libraries("${TARGET}" PRIVATE libdpcc  unity)

        if (UNIX AND NOT APPLE)
            target_compile_definitions("${TARGET}" PRIVATE _GNU_SOURCE)
            target_link_libraries("${TARGET}" PRIVATE coverage_config)
        endif()

        add_test(NAME "${TARGET}" COMMAND "${TARGET}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/run_tree"
        )
    endforeach(SRC)
endif()



add_test(NAME compile_test.py
    COMMAND python3 "${CMAKE_SOURCE_DIR}/tests/compile_test.py"
    DEPENDS "${CMAKE_SOURCE_DIR}/tests/compile_test.py"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

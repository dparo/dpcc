set(GENERATED_SOURCES_LIST
  "${CMAKE_SOURCE_DIR}/generated_srcs/src/gen.c"
  )

# add_custom_target(gen.py
#   python3 "${CMAKE_SOURCE_DIR}/generated_srcs/gen_all.py"
#   DEPENDS "${CMAKE_SOURCE_DIR}/generated_srcs/gen_all.py" "${CMAKE_SOURCE_DIR}/generated_srcs/utils.py" "${CMAKE_SOURCE_DIR}/generated_srcs/ops.py"
#   BYPRODUCTS "${CMAKE_SOURCE_DIR}/generated_srcs/last_gen.txt"
#   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/generated_srcs"
#   )


add_custom_target(gen.js
  node "${CMAKE_SOURCE_DIR}/generated_srcs/gen_all.js"
  DEPENDS "${CMAKE_SOURCE_DIR}/generated_srcs/gen_all.js" "${CMAKE_SOURCE_DIR}/generated_srcs/utils.py" "${CMAKE_SOURCE_DIR}/generated_srcs/ops.py"
  BYPRODUCTS "${CMAKE_SOURCE_DIR}/generated_srcs/last_gen.txt"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/generated_srcs"
  )


# add_dependencies(libdpcc gen.py)
add_dependencies(libdpcc gen.js)


foreach(SRC IN LISTS GENERATED_SOURCES_LIST)
  if (EXISTS "${SRC}")
    target_sources(libdpcc PRIVATE "${SRC}")
  endif()
endforeach(SRC)

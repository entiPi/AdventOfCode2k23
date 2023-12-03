function(setup_executable TARGET SOURCES)

  message("adding target ${TARGET} with sources ${SOURCES}")

  add_executable(${TARGET} ${SOURCES})

  target_compile_features(${TARGET}
    PRIVATE
      cxx_std_20
  )

  target_compile_options(${TARGET}
    PRIVATE
      -Werror
      -Wall
      -Wextra
      -Wpedantic
      -ggdb3
      -Wfloat-equal
      -Wnon-virtual-dtor
      -Wshadow
      -Wsuggest-override
      -Wfloat-conversion
  )

endfunction()

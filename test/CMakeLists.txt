if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEFAULT} -std=gnu99 -Wall -Wextra")
  if (CMAKE_CXX_COMPILER_WORKS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} ${CXX11_FLAG} -Wall -Wextra")

    if (MINGW)
      # see http://sourceforge.net/p/mingw/bugs/2250/
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__NO_INLINE__")
    endif ()
  endif ()
endif ()

include_directories(../include ../src)

set(TEST_SOURCES
    ordered-set.c
)

if (THEORIES)
  set(TEST_SOURCES ${TEST_SOURCES}
    asprintf.c
  )
endif ()

if (CMAKE_CXX_COMPILER_WORKS)
  set(TEST_SOURCES ${TEST_SOURCES}
    redirect.cc
  )
endif ()

add_executable(criterion_unit_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_link_libraries(criterion_unit_tests criterion)

add_dependencies(criterion_tests criterion_unit_tests)

add_test(criterion_unit_tests criterion_unit_tests)
set_property(TEST criterion_unit_tests PROPERTY
    ENVIRONMENT "CRITERION_NO_EARLY_EXIT=1" # for coverage
)

if (NOT MSVC) # we disable the scripted tests when building with MSVC
  add_custom_target(cram_tests)

  add_custom_command(TARGET cram_tests POST_BUILD
      COMMAND "${CMAKE_COMMAND}"
        -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}"
        -DCRAM_PATH="${CMAKE_CURRENT_SOURCE_DIR}/cram"
        -DENABLE_PATTERN_TESTS="${HAVE_PCRE}"
        -P "${CMAKE_MODULE_PATH}/Cram.cmake"
  )
  add_dependencies(cram_tests criterion_samples)

  add_test(cram_tests make cram_tests)

endif ()

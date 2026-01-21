# Set policy CMP0135 to NEW to avoid timestamp warnings
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


add_executable(core_tests
  matrix_tests.cpp
  expression_tests.cpp
  allocator_tests.cpp
)

target_link_libraries(core_tests
  PRIVATE
  MatrixCore
  GTest::gtest_main
)

if(CLANG_TIDY)
  set_target_properties(core_tests PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY}"
  )
endif()
if(CLANG_FORMAT)
  set_target_properties(core_tests PROPERTIES
    CXX_CLANG_FORMAT "${CLANG_FORMAT}"
  )
endif()

gtest_discover_tests(core_tests)

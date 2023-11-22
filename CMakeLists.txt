cmake_minimum_required(VERSION 3.22)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
project(magma VERSION ${PROJECT_VESRION})

project(magma LANGUAGES ASM_MASM C CXX)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include_directories(src)

add_library(
  magma_static
  STATIC
  src/gost_34_12_magma.asm
)

add_library(
  magma_dynamic
  SHARED
  src/gost_34_12_magma.cpp
)

add_executable(
  test_magma_static_gtest 
  tests/test_magma_static.cpp
)

add_executable(
  test_magma_dynamic_gtest 
  tests/test_magma_dynamic.cpp
)

add_executable(
  test_magma_gtest 
  tests/test_magma.cpp
)

add_executable(
  test_magma_speedtest 
  tests/speed_test/speed_test.cpp
)
  
target_link_libraries(
  test_magma_speedtest
  gtest 
  gtest_main
  magma_static)

target_link_libraries(
  test_magma_static_gtest
  gtest 
  gtest_main
  magma_static)

target_link_libraries(
  test_magma_dynamic_gtest
  gtest 
  gtest_main
  magma_dynamic)

target_link_libraries(
  test_magma_gtest
  gtest 
  gtest_main
  magma_static)

target_link_libraries(
  magma_dynamic
  magma_static)

set_target_properties(
  test_magma_static_gtest
  test_magma_dynamic_gtest
  test_magma_gtest
  test_magma_speedtest
  PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

if (MSVC)
  target_compile_options(test_magma_speedtest PRIVATE /W4)

  target_compile_options(test_magma_static_gtest PRIVATE /W4)

  target_compile_options(test_magma_gtest PRIVATE /W4)

  target_compile_options(test_magma_dynamic_gtest PRIVATE /W4)
else ()
  target_link_options(test_magma_speedtest PRIVATE -static-libgcc -static-libstdc++)

  target_link_options(test_magma_static_gtest PRIVATE -static-libgcc -static-libstdc++)

  target_link_options(test_magma_gtest PRIVATE -static-libgcc -static-libstdc++)

  target_link_options(test_magma_dynamic_gtest PRIVATE -static-libgcc -static-libstdc++)

  target_compile_options(
    test_magma_speedtest 
    PRIVATE
    -Wall 
    -Wextra 
    -pedantic 
  )

  target_compile_options(
    test_magma_static_gtest 
    PRIVATE
    -Wall 
    -Wextra 
    -pedantic 
  )

  target_compile_options(
    test_magma_gtest 
    PRIVATE
    -Wall 
    -Wextra 
    -pedantic 
  )

  target_compile_options(
    test_magma_dynamic_gtest 
    PRIVATE
    -Wall 
    -Wextra 
    -pedantic 
  )

  target_compile_options(
    magma_static 
    PRIVATE
    -Wall 
    -Wextra 
    -pedantic 
  )

  target_compile_options(
    magma_dynamic 
    PRIVATE
    -Wall 
    -Wextra 
    -pedantic 
  )
endif()

install(
    TARGETS 
    magma_dynamic
    RUNTIME DESTINATION magma/dynamic COMPONENT release
    LIBRARY DESTINATION magma/dynamic COMPONENT release
    ARCHIVE DESTINATION magma/dynamic COMPONENT release
)

install(
    TARGETS 
    magma_static
    RUNTIME DESTINATION magma/static COMPONENT release
    LIBRARY DESTINATION magma/static COMPONENT release
    ARCHIVE DESTINATION magma/static COMPONENT release
)

install(
  DIRECTORY include
  DESTINATION magma
  COMPONENT release
  FILES_MATCHING
  PATTERN "*.h"
)

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL release)

# CPack area
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_VERSION "0.0.2")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_CONTACT https://github.com/4l3x777/Magma)
include(CPack)

# Tests area
enable_testing()

add_test(
    NAME 
    unit_tests_test_magma_dynamic_gtest 
    COMMAND 
    test_magma_dynamic_gtest
)

add_test(
    NAME 
    unit_tests_test_magma_static_gtest 
    COMMAND 
    test_magma_static_gtest
)

add_test(
    NAME 
    unit_tests_test_magma_gtest 
    COMMAND 
    test_magma_gtest
)

#add_test(
#    NAME 
#    unit_tests_test_magma_speedtest 
#    COMMAND 
#    test_test_magma_speedtest
#)
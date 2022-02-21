# file: CMakeLists.txt
# date: 2021-02-01


enable_testing()


ExternalProject_Add(
  googletest
  PREFIX ${CMAKE_SOURCE_DIR}/build/_exts 
  GIT_REPOSITORY https://${GIT_HOST}/google/googletest.git  
  GIT_TAG release-1.11.0 
  GIT_PROGRESS TRUE
  DOWNLOAD_NO_PROGRESS FALSE
  CMAKE_ARGS 
      -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
      -DCMAKE_INSTALL_LIBDIR=lib
      -Dgtest_force_shared_crt=ON 
  LOG_DOWNLOAD ON 
  LOG_CONFIGURE ON 
  LOG_BUILD ON)
ExternalProject_Get_Property(googletest install_dir)
include_directories(${install_dir}/include)

add_library(libgoogletest::gtest_main STATIC IMPORTED)
set_property(
  TARGET libgoogletest::gtest_main 
  PROPERTY IMPORTED_LOCATION 
      ${install_dir}/lib/libgtest_main.a)

add_library(libgoogletest::gmock_main STATIC IMPORTED)
set_property(
  TARGET libgoogletest::gmock_main 
  PROPERTY IMPORTED_LOCATION  
      ${install_dir}/lib/libgmock_main.a)

add_library(libgoogletest::gtest STATIC IMPORTED)
set_property(
  TARGET libgoogletest::gtest 
  PROPERTY IMPORTED_LOCATION 
      ${install_dir}/lib/libgtest.a)

add_library(libgoogletest::gmock STATIC IMPORTED)
set_property(
  TARGET libgoogletest::gmock 
  PROPERTY IMPORTED_LOCATION  
      ${install_dir}/lib/libgmock.a)

add_library(libgoogletest::libgoogletest INTERFACE IMPORTED)
set_property(
  TARGET libgoogletest::libgoogletest 
  PROPERTY INTERFACE_LINK_LIBRARIES 
      libgoogletest::gtest_main libgoogletest::gmock_main 
      libgoogletest::gtest libgoogletest::gmock)
add_dependencies(libgoogletest::libgoogletest googletest)


add_executable(feather_tests test.cpp)
target_link_libraries(feather_tests PRIVATE feather)
target_link_libraries(feather_tests PRIVATE absl::strings)
target_link_libraries(feather_tests PRIVATE libgoogletest::libgoogletest)

add_test(NAME FEATHER_TESTS COMMAND ./test/cpp/feather_test)


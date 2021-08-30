# file: CMakeLists.txt
# date: 2021-02-01



execute_process(COMMAND bash -c "${CMAKEUP_VCPKG_BIN} install gtest")  
find_package(GTest CONFIG REQUIRED)

enable_testing()


add_executable(feather_tests test.cpp)
target_link_libraries(feather_tests PRIVATE feather)
target_link_libraries(feather_tests PRIVATE pybind11::module pybind11::lto pybind11::windows_extras pybind11::embed)
target_link_libraries(feather_tests PRIVATE absl::strings)
#target_link_libraries(feather_tests PRIVATE ${googletest})
target_link_libraries(feather_tests 
    PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)


add_test(NAME FEATHER_TESTS COMMAND ./test/cpp/feather_test)


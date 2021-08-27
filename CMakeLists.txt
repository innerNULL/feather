# file: CMakeLists.txt
# date: 2021-06-01

 
cmake_minimum_required(VERSION 3.14)
project(example)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS " -pthread -std=c++14 -funroll-loops -O3 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(PYTHON_EXECUTABLE "/usr/local/bin/python3") 


# Download cmakeup as pkg management.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/main.zip")
    message(STATUS "cmakeup has been downloaded.")
else()
    execute_process(
        COMMAND wget https://ghproxy.com/https://github.com/innerNULL/cmakeup/archive/refs/heads/main.zip
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
    execute_process(
        COMMAND unzip main.zip WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
endif()
set(CMKAEUP_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmakeup-main/cmakeup")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMKAEUP_ROOT_PATH}/module")
include(cmakeup)
cmakeup_init("${CMAKE_CURRENT_SOURCE_DIR}/build/_cmakeup_hub" "https://ghproxy.com/https://github.com")
cmakeup_root_path_register(${CMKAEUP_ROOT_PATH})
include(vcpkg_helpers)

integrate_vcpkg(
    "https://ghproxy.com/https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip"  
    "${CMAKE_CURRENT_SOURCE_DIR}/build"
)
message(STATUS "All vcpkg pkg are installed under ${CMAKEUP_VCPKG_ROOT}/packages")


cmakeup_pkg_cmake_importer("abseil" "abseil-cpp")
cmakeup_integrate_abseil_abseil_cpp("master" "null" "global")

cmakeup_pkg_cmake_importer("gabime" "spdlog")
cmakeup_integrate_gabime_spdlog("v1.x" "null" "global")

cmakeup_pkg_cmake_importer("nlohmann" "json")
cmakeup_integrate_nlohmann_json("master" "null" "global")

execute_process(COMMAND bash -c "${CMAKEUP_VCPKG_BIN} install pybind11")
find_package(pybind11 CONFIG REQUIRED)
include_directories(${pybind11_INCLUDE_DIRS})


include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src LIB_SOURCE)
add_library(feather STATIC ${LIB_SOURCE})


add_executable(example "./example.cpp")
target_link_libraries(example PRIVATE feather)
target_link_libraries(example PRIVATE pybind11::module pybind11::lto pybind11::windows_extras pybind11::embed)
target_link_libraries(example PRIVATE absl::strings)

pybind11_add_module(pyfeather ${LIB_SOURCE})
target_link_libraries(pyfeather PRIVATE absl::strings)

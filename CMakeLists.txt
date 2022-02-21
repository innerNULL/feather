# file: CMakeLists.txt
# date: 2021-06-01

 
cmake_minimum_required(VERSION 3.14)
project(example)

include(FetchContent)
include(ExternalProject)


set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS " -pthread -std=c++14 -funroll-loops -O3 -march=native -fPIC")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_CXX_FLAGS " -pthread -std=c++14 -funroll-loops -O3")
else()
  set(CMAKE_CXX_FLAGS " -pthread -std=c++14 -funroll-loops -O3 -march=native")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

option(FEATHER_BUILD_PY_BINDER "Build python binder" ON)
option(FEATHER_BUILD_TESTS "Build tests" ON)
option(FEATHER_BUILD_EXAMPLES "Build examples" ON)


execute_process(
  COMMAND bash -c "which python3" 
  OUTPUT_VARIABLE PYTHON3_ENV)
if(NOT "${PYTHON3_ENV}" STREQUAL "")
    string(REPLACE "\n" "" PYTHON3_ENV ""${PYTHON3_ENV})
endif()

set(PYTHON_EXECUTABLE 
  ${PYTHON3_ENV} CACHE STRING "PYTHON_EXECUTABLE")
message(STATUS "PYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}")


set(FETCHCONTENT_QUIET FALSE)
set(GIT_HOST "ghproxy.com/https://github.com")

set(VCPKG_FEATURE_FLAGS "versions")
set(VCPKG_ROOT ${FETCHCONTENT_BASE_DIR}/vcpkg-src)
set(_VCPKG_BIN ${VCPKG_ROOT}/vcpkg)
set(_VCPKG_CMAKE 
  ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_TOOLCHAIN_FILE ${_VCPKG_CMAKE})

if (EXISTS ${_VCPKG_BIN})
  message(
    STATUS 
    "vcpkg already located at ${_VCPKG_BIN}")
  set(_VCPKG_EXISTS TRUE)
else()
  message(STATUS "downloading vcpkg first")
  FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://${GIT_HOST}/microsoft/vcpkg.git  
    GIT_TAG 2022.02.02
    GIT_PROGRESS TRUE 
    DOWNLOAD_NO_PROGRESS FALSE
    PATCH_COMMAND 
        cd <SOURCE_DIR> &&  
        bash <SOURCE_DIR>/bootstrap-vcpkg.sh)
  FetchContent_MakeAvailable(vcpkg)
endif()

include(${_VCPKG_CMAKE})
execute_process(
  COMMAND 
      ${_VCPKG_BIN} integrate install 
        --x-manifest-root=${CMAKE_SOURCE_DIR})
find_package(absl CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(xxHash CONFIG REQUIRED)


include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src LIB_SOURCE)

add_library(feather STATIC ${LIB_SOURCE})
target_link_libraries(feather 
  PRIVATE absl::strings)
target_link_libraries(feather 
  PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(feather 
  PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(feather 
  PRIVATE xxHash::xxhash) 


if (${FEATHER_BUILD_EXAMPLES})
  add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/cpp)
endif()

if (${FEATHER_BUILD_PY_BINDER})
  add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python)
endif()

if (${FEATHER_BUILD_TESTS})
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/cpp)
endif()

# file: CMakeLists.txt
# date: 2022-02-21


FetchContent_Declare(
  pybind_pybind11
  GIT_REPOSITORY https://${GIT_HOST}/pybind/pybind11.git 
  GIT_TAG v2.9.0 
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(pybind_pybind11)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src PYBINDER)


pybind11_add_module(pyfeather ${LIB_SOURCE} ${PYBINDER})
target_link_libraries(pyfeather PRIVATE absl::strings)
target_link_libraries(pyfeather PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(pyfeather PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(pyfeather PRIVATE xxHash::xxhash)

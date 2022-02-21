# file: CMakeLists.txt
# date: 2022-02-22


add_executable(
  example 
  ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp)
target_link_libraries(
  example 
  PRIVATE feather)
target_link_libraries(
  example 
  PRIVATE absl::strings)
target_link_libraries(
  example 
  PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(
  example 
  PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(
  example 
  PRIVATE xxHash::xxhash)

/// file: core.cpp


#include <iostream>
#include <fstream>
#include <functional>
#include <absl/strings/str_split.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>

#include "feather/FeaHash.h"

namespace py = pybind11;


namespace feather {


FeaHash::FeaHash() {
  spdlog::info("Initializing `FeaHash`..."); 
}


FeaHash::FeaHash(const std::string& conf_path) {
  spdlog::info("Initializing `FeaHash` with config '{0}'...", conf_path);
  this->fea_hash["meta"]["conf_path"] = conf_path;

  std::string line;
  std::ifstream conf_file(conf_path);
  while (std::getline(conf_file, line)) {
    std::vector<std::string> conf_line = absl::StrSplit(line, " ");
    this->fea_hash["slots"][conf_line[0]] = {};
    this->fea_hash["slots"][conf_line[0]]["id"] = std::stoi(conf_line[1]);
    this->fea_hash["slots"][conf_line[0]]["bucket_num"] = std::stoi(conf_line[2]);

    this->SlotRegister(
        conf_line[0], std::stoi(conf_line[1]), std::stoi(conf_line[2]));
  }
  //std::cout << this->fea_hash.dump(4) << std::endl;
}


int64_t FeaHash::Fea2FeaID(
    const std::string& name, const std::string& val) {
  FeaSlot* fea_slot = &(this->name2slot[name]);
  int32_t slot_id = fea_slot->GetSlotID();
  int32_t bucket_id = fea_slot->GetBucketID(val);
  std::string fea_id_str = std::to_string(slot_id) + std::to_string(bucket_id);
  return std::stol(fea_id_str);
}


int32_t FeaHash::SlotRegister(const std::string& fea_name, 
    const int32_t slot_id, const int32_t bucket_size) {
  if (this->name2slot.find(fea_name) == this->name2slot.end()) {
    this->name2slot.emplace(fea_name, FeaSlot(fea_name, slot_id, bucket_size));
    this->slot2name[slot_id] = fea_name;
  }
  this->slot_num += 1;
  spdlog::info("Registered feature '{0}' to slot {1} with {2} as bucket-size.", 
      fea_name, slot_id, bucket_size);
  return 0;
}


int64_t FeaHash::FeaRegister(
    const std::string& fea_name, const std::string& fea_value) {
  int64_t val_hash = std::hash<std::string>()(fea_value);
  this->name2slot.find(fea_name)->second.ValRegister(fea_value, val_hash);
  return this->Fea2FeaID(fea_name, fea_value);
}


void FeaHash_pybind(py::module& m) {
  py::class_<FeaHash>(m, "FeaHash")
      .def(py::init<>())
      .def(py::init<const std::string&>())
      .def("FeaRegister", &FeaHash::FeaRegister)
      .def("SlotRegister", &FeaHash::SlotRegister);
}


} // namespace feather

/// file: core.cpp


#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <absl/strings/str_split.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

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
  int32_t slot_digits = -1;
  while (std::getline(conf_file, line)) {
    std::vector<std::string> conf_line = absl::StrSplit(line, " ");
    this->fea_hash["slots"][conf_line[0]] = {};
    this->fea_hash["slots"][conf_line[0]]["id"] = std::stoi(conf_line[1]);
    this->fea_hash["slots"][conf_line[0]]["bucket_num"] = std::stoi(conf_line[2]);
    this->fea_hash["slots"][conf_line[0]]["type"] = std::stoi(conf_line[3]);

    if (slot_digits != -1 && conf_line[1].size() != slot_digits) {
      throw "Slot ID illegal, each slot-id string should has same length/digits.";
    }
    slot_digits = conf_line[1].size();

    this->SlotRegister(conf_line[0], std::stoi(conf_line[1]), 
        std::stoi(conf_line[2]), std::stoi(conf_line[3]));
  }

  //std::cout << this->fea_hash.dump(4) << std::endl;
}


std::string FeaHash::BucketID2BucketCode(const int32_t bucket_id) {
  std::string bucket_id_str = std::to_string(bucket_id);
  if (bucket_id_str.size() == this->val_hash_digits) {
    return bucket_id_str;
  } else if (bucket_id_str.size() < this->val_hash_digits) {
    std::string zero_sup(this->val_hash_digits - bucket_id_str.size(), '0');
    return zero_sup + bucket_id_str;
  } else {
    /// This should never happen
    throw "Max bucket-code digits number should not larget than the size of largets slot-id.";
  } 
}


std::vector<int64_t> FeaHash::Fea2FeaID(
    const std::string& name, const std::string& val) {
  std::vector<int64_t> fea_id;
  std::string fea_id_str;
  FeaSlot* fea_slot = &(this->name2slot[name]);
  int16_t fea_type = this->fea_hash["slots"][name]["type"]; 
  int32_t slot_id = fea_slot->GetSlotID();
  int32_t bucket_id = fea_slot->GetBucketID(val);

  if (fea_type == 0) {
    fea_id_str = std::to_string(slot_id) + this->BucketID2BucketCode(bucket_id);
  } else if (fea_type == 1) {
    //fea_id_str = std::to_string(slot_id) + std::string(this->val_hash_digits, '0');
    fea_id_str = std::to_string(slot_id) + this->BucketID2BucketCode(0);
  } else if (fea_type == 2) {
    /// Never happen
  }
  fea_id.emplace_back(std::stol(fea_id_str));
  return fea_id;
}


int32_t FeaHash::SlotRegister(const std::string& fea_name, 
    const int32_t slot_id, const int32_t bucket_size, 
    const int16_t slot_type) {
  if (this->name2slot.find(fea_name) == this->name2slot.end()) {
    this->name2slot.emplace(
        fea_name, FeaSlot(fea_name, slot_id, bucket_size, slot_type));
    this->slot2name[slot_id] = fea_name;
  }
  this->slot_num += 1;
  //this->max_slot = std::max(this->max_slot, slot_id);
  //this->min_slot = std::min(this->max_slot, slot_id);
  this->val_hash_digits = std::max(
      (int32_t)std::to_string(bucket_size).size(), this->val_hash_digits);
  spdlog::info(
      "Registered type {0} feature '{1}' to slot {2} with {3} as bucket-size.", 
      slot_type, fea_name, slot_id, bucket_size);
  return 0;
}


std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const std::string& fea_value) {
  if (this->name2slot.find(fea_name) == this->name2slot.end()) {
    throw "Feature " + fea_name + " should be registered to a slot first";
  }
  FeaSlot* slot = &(this->name2slot[fea_name]);
  if (this->fea_hash["slots"][fea_name]["type"] == 0) {
    int64_t val_hash = std::hash<std::string>()(fea_value);
    this->name2slot.find(fea_name)->second.ValRegister(fea_value, val_hash);
    return this->Fea2FeaID(fea_name, fea_value);
  } else if (this->fea_hash["slots"][fea_name]["type"] == 1) {
    return this->Fea2FeaID(fea_name, fea_value);
  } else {
    throw "Feature " + fea_name + " should be registered as type 0 or 1."; 
  }
}


std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const float fea_value) {
  if (this->name2slot.find(fea_name) != this->name2slot.end()) {
    return this->Fea2FeaID(fea_name, std::to_string(fea_value)); 
  } else {
    throw "Feature " + fea_name + " should be registered to a slot first";
  }
}


std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const std::vector<float>& fea_value) {
  if (this->name2slot.find(fea_name) == this->name2slot.end()) {
    throw "Feature " + fea_name + " should be registered to a slot first";
  } else if (this->name2slot[fea_name].GetBucketSize() != fea_value.size()) {
    throw "Size or array feature " + fea_name 
        + " should be same with its registered slot's bucket-size.";  
  } else {
    int32_t slot_id = this->name2slot[fea_name].GetSlotID();
    std::vector<int64_t> fea_id;
    fea_id.resize(fea_value.size());
    for (int32_t i = 0; i < fea_value.size(); ++i) {
      fea_id[i] = std::stoi( std::to_string(slot_id) + this->BucketID2BucketCode(i) );
    }
    return fea_id;
  }
}


const nlohmann::json& FeaHash::GetMeta() {
  return this->fea_hash;
}


void FeaHash_pybind(py::module& m) {
  py::class_<FeaHash>(m, "FeaHash")
      .def(py::init<>())
      .def(py::init<const std::string&>())
      .def("SlotRegister", &FeaHash::SlotRegister)
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::string&)>(&FeaHash::FeaRegister))
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const float)>(&FeaHash::FeaRegister))
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::vector<float>&)>(&FeaHash::FeaRegister));
}


} // namespace feather

/// file: core.cpp


#include <memory>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <absl/strings/str_split.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "feather/BiDict.h"
#include "feather/utils.h"
#include "feather/FeaHash.h"

namespace py = pybind11;


namespace feather {


FeaHash::FeaHash() {
  spdlog::info("Initializing `FeaHash`..."); 
}


FeaHash::FeaHash(const std::string& conf_path, const bool reversible) {
  spdlog::info("Initializing `FeaHash` with config '{0}'...", conf_path);
  //this->fea_hash["meta"]["conf_path"] = conf_path;

  std::string line;
  std::ifstream conf_file(conf_path);
  int32_t slot_digits = -1;
  while (std::getline(conf_file, line)) {
    std::vector<std::string> conf_line = absl::StrSplit(line, " ");
    this->slot2name_dict_->Register({conf_line[0], conf_line[1]});

    if (slot_digits != -1 && conf_line[1].size() != slot_digits) {
      throw "Slot ID illegal, each slot-id string should has same length/digits.";
    }
    slot_digits = conf_line[1].size();
    this->SlotRegister(conf_line[0], std::stoi(conf_line[1]), 
        std::stoi(conf_line[2]), std::stoi(conf_line[3]));
  }
  if (reversible) { this->Hash2IndexDictBuild(conf_path); }
}


void FeaHash::Hash2IndexDictBuild(const std::string& conf_path) {
  std::string line;
  std::ifstream conf_file(conf_path);
  int32_t count = 1; // index recorder.
  while (std::getline(conf_file, line)) {
    std::vector<std::string> conf_line = absl::StrSplit(line, " ");
    int64_t slot_id = std::stoi(conf_line[1]);
    int32_t bucket_num = std::stoi(conf_line[2]);
    for (int32_t i = 0; i < bucket_num; ++i) {
      int64_t fea_hash = slot_id * pow(10, this->val_hash_digits) + i;
      this->hash2index_dict_->Register(
          {std::to_string(fea_hash), std::to_string(count++)});
    }
  }
}


std::string FeaHash::FeaHash2FeaIndex(const int64_t fea_hash) {
  return this->hash2index_dict_->Map(
      "fea_hash", "fea_index", std::to_string(fea_hash))[0];
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


int16_t FeaHash::FeaValCheck(const std::string& name) {
  if (this->name2slot.find(name) == this->name2slot.end()) {
    throw "Feature " + name + " should be registered to a slot first";
  } 
  return 0;
}


int16_t FeaHash::FeaValCheck(
    const std::string& name, FeaValue* val) {
  if (this->FeaValCheck(name) != 0) {
    return this->FeaValCheck(name);
  } else if (val->GetType() == 2 && val != nullptr) {
    if (this->name2slot[name].GetBucketSize() 
        != val->GetVecValue()->size()) {
      throw "Size or vector feature " + name 
          + " should be same with its slot's bucket-size.";  
    }
  }
  return 0;
}


std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const std::string& fea_value) {
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  if (this->FeaValCheck(fea_name) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    FeaValue fea_value_(fea_value, fea_type);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
}


std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const int32_t fea_value) {
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  if (this->FeaValCheck(fea_name) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    FeaValue fea_value_(fea_value, fea_type);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
}



std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const float fea_value) {
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  if (this->FeaValCheck(fea_name) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    FeaValue fea_value_(fea_value, fea_type);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
}


std::vector<int64_t> FeaHash::FeaRegister(
    const std::string& fea_name, const std::vector<float>& fea_value) {
  std::vector<int64_t> fea_val_hash;
  FeaValue fea_value_(fea_value);
  if (this->FeaValCheck(fea_name, &fea_value_) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
}


std::string FeaHash::FeaHash2FeaName(const int64_t fea_hash) {
  std::string fea_hash_str = std::to_string(fea_hash);
  std::string slot = fea_hash_str.substr(
      0, fea_hash_str.size() - this->val_hash_digits);
  return this->slot2name_dict_->Map("slot", "fea_name", slot)[0];
}


const std::unordered_map<std::string, FeaSlot>* FeaHash::GetSlots() {
  return &(this->name2slot);
}


/*
const nlohmann::json& FeaHash::GetMeta() {
  return this->fea_hash;
}
*/


std::vector<int32_t> FeaHash::FeaVal2FeaHashBucket(
    FeaValue* fea_val, FeaSlot* fea_slot) {
  std::vector<int32_t> hash_bucket_id;
  std::vector<int64_t> fea_hash = fea_val->GetHash();

  hash_bucket_id.resize(fea_hash.size());
  if (fea_val->GetType() == 1 && (
      fea_hash.size() != 1 || fea_slot->GetBucketSize() != 1)) {
    throw "Continuous feature's hash-bucket size must be 1";
  }
  if (fea_val->GetType() == 2 
      && fea_hash.size() != fea_slot->GetBucketSize()) {
    throw "Vector feature's hash-bucket size must be same to its dim";
  }
  /// TODO@202108271359: Using SSE or AVX
  for (int32_t i=0; i < fea_hash.size(); ++i) {
    /// TODO@202108271400: Too much memory for id style feature.
    // hash_bucket_id[i] = fea_slot->ValRegister(
    //     fea_val->GetDescreteValue(), fea_hash[i]); 
    hash_bucket_id[i] = abs(fea_hash[i] % fea_slot->GetBucketSize()); 
  }
  return hash_bucket_id;
}


std::vector<std::string> FeaHash::FeaVal2FeaHashBucketCode(
    FeaValue* fea_val, FeaSlot* fea_slot) {
  std::vector<std::string> fea_bucket_code;
  std::vector<int32_t> hash_bucket_id = this->FeaVal2FeaHashBucket(
      fea_val, fea_slot);
  fea_bucket_code.resize(hash_bucket_id.size());
  for (int32_t i = 0; i < hash_bucket_id.size(); ++i) {
    fea_bucket_code[i] = num2str_code<int32_t>(
        hash_bucket_id[i], this->val_hash_digits);
  }
  return fea_bucket_code;
}


std::vector<int64_t> FeaHash::FeaVal2FeaHash(
    FeaValue* fea_val, FeaSlot* fea_slot) {
  std::vector<int64_t> fea_val_hash;
  int32_t slot_id = fea_slot->GetSlotID();
  std::vector<std::string> fea_bucket_code = this->FeaVal2FeaHashBucketCode(
      fea_val, fea_slot);
  fea_val_hash.resize(fea_bucket_code.size());
  for (int32_t i = 0; i < fea_bucket_code.size(); ++i) {
    fea_val_hash[i] = std::stol(
        std::to_string(slot_id) + fea_bucket_code[i]);
  }
  return fea_val_hash;
}


void FeaHash_pybind(py::module& m) {
  py::class_<FeaHash>(m, "FeaHash")
      .def(py::init<>())
      .def(py::init<const std::string&>())
      .def("SlotRegister", &FeaHash::SlotRegister)
      .def("FeaHash2FeaName", &FeaHash::FeaHash2FeaName)
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::string&)>(&FeaHash::FeaRegister))
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const int32_t)>(&FeaHash::FeaRegister))
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const float)>(&FeaHash::FeaRegister))
      .def("FeaRegister", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::vector<float>&)>(&FeaHash::FeaRegister));
}


} // namespace feather

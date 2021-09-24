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

#include "feather/BiDict.h"
#include "feather/utils.h"
#include "feather/FeaHash.h"


namespace feather {


FeaHash::FeaHash() {
  spdlog::info("Initializing `FeaHash`..."); 
}


FeaHash::FeaHash(const std::string& conf_path, const bool reversible) {
  spdlog::info("Initializing `FeaHash` with config '{0}'...", conf_path);

  std::string line;
  std::ifstream conf_file(conf_path);
  while (std::getline(conf_file, line)) {
    std::vector<std::string> conf_line = absl::StrSplit(line, " ");
    this->SchemaLineRegister(conf_line);
  }
  //if (reversible) { this->Hash2IndexDictBuild(conf_path); }
  if (reversible) { this->Hash2IndexDictBuild(); }
}


void FeaHash::Hash2IndexDictBuild() {
  std::map<int64_t, int32_t> slot_sorter;
  for (auto iter = this->name2slot.begin(); 
      iter != this->name2slot.end(); ++iter) {
    const FeaSlot* slot = &(iter->second);
    int64_t slot_id = slot->GetSlotID();
    int32_t bucket_num = slot->GetBucketSize();
    slot_sorter[slot_id] = bucket_num;
  }
  for (auto iter = slot_sorter.begin(); 
      iter != slot_sorter.end(); iter++) {
    int64_t slot_id = iter->first;
    int32_t bucket_num = iter->second;
    for (int32_t i = 0; i < bucket_num; ++i) {
      int64_t fea_hash = slot_id * pow(10, this->bucket_code_digits_) + i;
      this->hash2index_dict_->Register(
          {std::to_string(fea_hash), std::to_string(this->fea_index++)});
    }
  }
}


void FeaHash::SchemaLineRegister(
    const std::vector<std::string>& schema_line) {
  std::string fea_name = schema_line[0];
  int64_t slot_id = std::stol(schema_line[1]);
  int32_t bucket_num = std::stoi(schema_line[2]);
  int8_t fea_type = std::stoi(schema_line[3]);
  std::string slot_id_str = std::to_string(slot_id);
  if (this->slot_digits != -1 
      && (slot_id_str.size() != this->slot_digits 
        || slot_id_str.size() != schema_line[1].size())
  ) {
    spdlog::error(
        "Slot ID illegal, each slot-id should has same digits, \
        or its hight digit could not be zero.");
    throw "Slot ID illegal.";
  }
  this->slot2name_dict_->Register({fea_name, slot_id_str});
  this->slot_digits = slot_id_str.size();
  this->SlotRegister(fea_name, slot_id, bucket_num, fea_type);
}


std::string FeaHash::FeaHash2FeaIndexStr(const int64_t fea_hash) {
  return this->hash2index_dict_->Map(
      "fea_hash", "fea_index", std::to_string(fea_hash))[0];
}


const FeaSlot* FeaHash::GetSlot(const std::string& fea_name) {
  FeaSlot* slot = nullptr;
  if (this->name2slot.find(fea_name) != this->name2slot.end()) {
    slot = &(this->name2slot[fea_name]);
  }
  return slot;
}


int32_t FeaHash::SlotRegister(const std::string& fea_name, 
    const int32_t slot_id, const int32_t bucket_size, 
    const int16_t slot_type) {
  if (this->name2slot.find(fea_name) == this->name2slot.end()) {
    this->name2slot.emplace(
        fea_name, FeaSlot(fea_name, slot_id, bucket_size, slot_type));
  }
  this->slot_num += 1;
  this->bucket_code_digits_ = std::max(
      (int32_t)std::to_string(bucket_size).size(), this->bucket_code_digits_);
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


std::vector<int64_t> FeaHash::GetFeaHash(
    const std::string& fea_name, const std::string& fea_value) {
  /** TODO@202109011527: Try using template func.
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  if (this->FeaValCheck(fea_name) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    FeaValue fea_value_(fea_value, fea_type);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
  */
  return this->Fea2FeaHash<std::string>(fea_name, fea_value);
}


std::vector<int64_t> FeaHash::GetFeaHash(
    const std::string& fea_name, const int32_t fea_value) {
  /** TODO@202109011527: Try using template func.
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  if (this->FeaValCheck(fea_name) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    FeaValue fea_value_(fea_value, fea_type);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
  */
  return this->Fea2FeaHash<int32_t>(fea_name, fea_value);
}



std::vector<int64_t> FeaHash::GetFeaHash(
    const std::string& fea_name, const float fea_value) {
  /** TODO@202109011527: Try using template func. 
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  if (this->FeaValCheck(fea_name) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    FeaValue fea_value_(fea_value, fea_type);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
  */
  return this->Fea2FeaHash<float>(fea_name, fea_value);
}


std::vector<int64_t> FeaHash::GetFeaHash(
    const std::string& fea_name, const std::vector<float>& fea_value) {
  /** TODO@202109011527: Try using template func. 
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  FeaValue fea_value_(fea_value, fea_type);
  if (this->FeaValCheck(fea_name, &fea_value_) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
  */
  return this->Fea2FeaHash< std::vector<float> >(fea_name, fea_value);
}


std::vector<int64_t> FeaHash::GetFeaHash(
    const std::string& fea_name, const std::vector<std::string>& fea_value) {
  /** TODO@202109011527: Try using template func.  
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  FeaValue fea_value_(fea_value, fea_type);
  if (this->FeaValCheck(fea_name, &fea_value_) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
  */
  return this->Fea2FeaHash< std::vector<std::string> >(fea_name, fea_value);
}


std::vector<int64_t> FeaHash::GetFeaHash(
    const std::string& fea_name, const std::vector<int32_t>& fea_value) {
  /** TODO@202109011527: Try using template func.
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  FeaValue fea_value_(fea_value, fea_type);
  if (this->FeaValCheck(fea_name, &fea_value_) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
  */
  return this->Fea2FeaHash< std::vector<int32_t> >(fea_name, fea_value);
}


template<typename VAL_TYPE>
std::vector<int64_t> FeaHash::Fea2FeaHash(
    const std::string& fea_name, const VAL_TYPE& fea_value) {
  std::vector<int64_t> fea_val_hash;
  int8_t fea_type = this->name2slot[fea_name].GetType();
  FeaValue fea_value_(fea_value, fea_type);
  if (this->FeaValCheck(fea_name, &fea_value_) == 0) {
    FeaSlot* fea_slot_ = &(this->name2slot[fea_name]);
    fea_val_hash = this->FeaVal2FeaHash(&fea_value_, fea_slot_);
  }
  return fea_val_hash;
}


std::string FeaHash::FeaHash2FeaName(const int64_t fea_hash) {
  std::string fea_hash_str = std::to_string(fea_hash);
  std::string slot = fea_hash_str.substr(
      0, fea_hash_str.size() - this->bucket_code_digits_);
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


int32_t FeaHash::GetFeaBucketCodeLength() {
  return this->bucket_code_digits_;
}


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
    hash_bucket_id[i] = llabs(fea_hash[i]) % fea_slot->GetBucketSize();
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
        hash_bucket_id[i], this->bucket_code_digits_);
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


} // namespace feather

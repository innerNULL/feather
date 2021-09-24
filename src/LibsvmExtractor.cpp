/// file: LibsvmExtractor.cpp


#include <iostream>
#include <map>
#include <algorithm>
#include <thread>
#include <spdlog/spdlog.h>

#include "feather/utils.h"
#include "feather/FeaValue.h"
#include "feather/LibsvmExtractor.h"


namespace feather {


LibsvmExtractor::LibsvmExtractor(const std::string& feahash_conf, 
    const std::string& label, const bool index) {
  this->index = index;
  if (index) {
    /// TODO@202108201502
    spdlog::warn(
        "`LibsvmExtractor` will convert feature-hash to index maginitude...");
  } 
  this->fea_hash = FeaHash(feahash_conf);
  this->label = label;
}


std::string LibsvmExtractor::Extract(
    const nlohmann::json& flat_json, const bool with_label) {
  std::string output;
  const std::unordered_map<std::string, FeaSlot>* slots_ = 
      this->fea_hash.GetSlots();
  /// Using ordered map, default ranking by index/hash
  std::map<int64_t, std::string> libsvm_kv;
 
  for (auto it = slots_->begin(); it != slots_->end(); ++it) {
    std::string slot_name = it->first;
    FeaSlot slot = it->second; 
    const int8_t slot_type = it->second.GetType();
    if (flat_json.find(slot_name) == flat_json.end()) { continue; }
        
    /// Feature-Value hash-ids.
    auto json_val = flat_json[slot_name]; /// val
    FeaValue* fea_val = this->JsonVal2FeaVal(slot_type, json_val);
    std::vector<int64_t> hash_id = this->fea_hash.FeaVal2FeaHash(fea_val, &slot);

    for (int32_t i = 0; i < hash_id.size(); ++i) {
      int64_t id;
      if (this->index) {
        id = std::stol(this->fea_hash.FeaHash2FeaIndexStr(hash_id[i]));
      } else { 
        id = hash_id[i];
      }
      if (slot_type == 0) { libsvm_kv[id] = "1"; }
      if (slot_type == 1) { 
        libsvm_kv[id] = std::to_string(fea_val->GetContinuous()); 
      }
      if (slot_type == 2) {
        libsvm_kv[id] = std::to_string((float)json_val[i]); 
      }
    }
  }
  for (auto it = libsvm_kv.begin(); it != libsvm_kv.end(); ++it) {
    output = output + std::to_string(it->first) + ":" + it->second + " ";
  }
  if (with_label) {
    return this->ExtractLabel(flat_json) + " " + output;
  } else {
    return output;
  }
}


std::string LibsvmExtractor::Extract(
    const std::string& flat_json, const bool with_label) {
  nlohmann::json flat_json_obj = nlohmann::json::parse(flat_json);
  return this->Extract(flat_json_obj, with_label);
}


std::vector<std::string> LibsvmExtractor::BatchExtract(
    const std::vector<nlohmann::json>& flat_json, const bool with_label) {
  std::vector<std::string> output;
  std::vector<std::thread> threads_;
  output.resize(flat_json.size());
  threads_.resize(flat_json.size());
  for (int32_t i = 0; i < flat_json.size(); ++i) {
    std::thread curr_threads_([this, &output, &flat_json, with_label, i] {
        output[i] = this->Extract(flat_json[i], with_label); 
    });
    threads_[i] = std::move(curr_threads_);
  }
  for (auto& thread_ : threads_) { thread_.join(); }
  return output;
}


std::vector<std::string> LibsvmExtractor::BatchExtract(
    const std::vector<std::string>& json_str, const bool with_label) {
  std::vector<nlohmann::json> json_list;
  json_list.resize(json_str.size());
  for (int32_t i = 0; i < json_str.size(); ++i) {
    json_list[i] = nlohmann::json::parse(json_str[i]);
  }
  return this->BatchExtract(json_list, with_label);
}



std::string LibsvmExtractor::ExtractLabel(
    const nlohmann::json& flat_json) {
  std::string label_str = "null";
  if (flat_json.find(this->label) != flat_json.end()) {
    label_str = "error";
    if (flat_json[this->label].is_string()) {
      label_str = flat_json[this->label]; 
    } else if (flat_json[this->label].is_number_integer()) {
      label_str = std::to_string(
          flat_json[this->label].get<int32_t>());
    } else if (flat_json[this->label].is_number_float()) {
      label_str = std::to_string(
          flat_json[this->label].get<float>());
    }
  }
  return label_str;
}


FeaValue* LibsvmExtractor::JsonVal2FeaVal(
    const int8_t type, const nlohmann::json& json_val) {
  FeaValue* fea_val = new FeaValue();
  if (json_val.is_array()) {
    if (json_val.size() == 0) {
      spdlog::error("Vector value could not be a empty array.");
      throw "`json_val` error";
    }
    if (json_val[0].is_string()) {
      std::vector<std::string> val = json_val.get< std::vector<std::string> >();
      fea_val = new FeaValue(val, type);
    } else if (json_val[0].is_number_float()) {
      std::vector<float> val = json_val.get< std::vector<float> >();
      fea_val = new FeaValue(val, type);
    } else if (json_val[0].is_number_integer()) {
      std::vector<int32_t> val = json_val.get< std::vector<int32_t> >();
      fea_val = new FeaValue(val, type);
    } else {
      std::vector<std::string> val = json_val.get< std::vector<std::string> >();
      fea_val = new FeaValue(val, type);
    }
  } else if (json_val.is_string()) {
    std::string val = json_val.get<std::string>();
    fea_val = new FeaValue(val, type);
  } else if (json_val.is_number_integer()) {
    int32_t val = json_val.get<int32_t>();
    fea_val = new FeaValue(val, type);
  } else if (json_val.is_number_float()) {
    float val = json_val.get<float>();
    fea_val = new FeaValue(val, type);
  } else if (json_val.is_number()) {
    /* TODO@202109021800: Waiting to drop.
    if (json_val.get<float>() == (float)json_val.get<int>()) {
      int32_t val = json_val.get<int32_t>();
      fea_val = new FeaValue(val, type); 
    } else {
      float val = json_val.get<float>();
      fea_val = new FeaValue(val, type);
    }
    */
  }
  return fea_val;
}


} // namespace feather

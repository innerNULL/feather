/// file: LibsvmExtractor.cpp


#include <iostream>
#include <map>
#include <spdlog/spdlog.h>

#include "feather/FeaValue.h"
#include "feather/LibsvmExtractor.h"


namespace feather {


LibsvmExtractor::LibsvmExtractor(const std::string& feahash_conf, 
    const std::string& label, const bool compress) {
  if (compress) {
    /// TODO@202108201502
    spdlog::warn("Now `LibsvmExtractor` not support 'compress' mode...");
  } else {
    this->fea_hash = FeaHash(feahash_conf);
    this->label = label;
  }
}


std::string LibsvmExtractor::Extract(const nlohmann::json& flat_json) {
  std::string output;
  std::string label_str = "null";
  const std::unordered_map<std::string, FeaSlot>* slots_ = 
      this->fea_hash.GetSlots();
  std::map<std::string, std::string> libsvm_kv;
  
  for (auto it = slots_->begin(); it != slots_->end(); ++it) {
    std::string slot_name = it->first;
    const int8_t slot_type = it->second.GetType();
    if (flat_json.find(slot_name) == flat_json.end()) { continue; }
        
    std::vector<int64_t> hash_id; /// Feature-Value hash-id.
    auto json_val = flat_json[slot_name]; /// val
    std::string str_val;
  
    if (json_val.is_array()) {
      std::vector<float> val = json_val.get< std::vector<float> >();
      hash_id = this->fea_hash.FeaRegister(slot_name, val);
    } else if (json_val.is_string()) {
      str_val = json_val.get<std::string>();
      hash_id = this->fea_hash.FeaRegister(slot_name, str_val); 
    } else if (json_val.is_number()) {
      if (json_val.get<float>() == (float)json_val.get<int>()) {
        str_val = std::to_string(json_val.get<int32_t>());
      } else {
        str_val = std::to_string(json_val.get<float>());
      }
      hash_id = this->fea_hash.FeaRegister(slot_name, str_val);
    }

    for (int32_t i = 0; i < hash_id.size(); ++i) {
      std::string hash_str = std::to_string(hash_id[i]);
      if (slot_type == 0) { libsvm_kv[hash_str] = "1"; }
      if (slot_type == 1) { libsvm_kv[hash_str] = str_val; }
      if (slot_type == 2) {
        libsvm_kv[hash_str] = std::to_string((float)json_val[i]); 
      }
    }
  }
  for (auto it = libsvm_kv.begin(); it != libsvm_kv.end(); ++it) {
    output = output + it->first + ":" + it->second + " ";
  }
  if (flat_json.find(this->label) != flat_json.end()) {
    label_str = "error";
    if (flat_json[this->label].is_string()) { label_str = flat_json[this->label]; }
    if (flat_json[this->label].is_number()) {
      label_str = std::to_string((float)flat_json[this->label]);
    }
  }
  return label_str + " " + output;
}


std::string LibsvmExtractor::Extract(const std::string& flat_json) {
  nlohmann::json flat_json_obj = nlohmann::json::parse(flat_json);
  return this->Extract(flat_json_obj);
}


void LibsvmExtractor_pybind(py::module &m) {
  py::class_<LibsvmExtractor>(m, "LibsvmExtractor")
      .def(py::init<>())
      .def(py::init<const std::string&, const std::string&, const bool>())
      .def("Extract", 
          static_cast<std::string (LibsvmExtractor::*)(const std::string&)>(
            &LibsvmExtractor::Extract));
}


} // namespace feather

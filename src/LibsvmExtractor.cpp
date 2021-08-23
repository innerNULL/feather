/// file: LibsvmExtractor.cpp


#include <iostream>
#include <spdlog/spdlog.h>

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
  const nlohmann::json& slots = this->fea_hash.GetMeta()["slots"];
  //std::cout << slots.dump(4) << std::endl;
  
  for (auto slot = slots.begin(); slot != slots.end(); ++slot) {
    std::string slot_name = slot.key();
    int16_t slot_type = slot.value()["type"];

    if (flat_json.find(slot_name) == flat_json.end()) { continue; }
        
    std::vector<int64_t> hash_id; /// Feature-Value hash-id.
    std::string fea_val; /// String form feature value.
    std::vector<std::string> libsvm_val; /// Hashed feature value for libsvm data format.
    auto val = flat_json[slot_name]; /// val`

    if (val.is_number()) {
      if (slot_type == 1) {
        fea_val = std::to_string((float)val);
        libsvm_val.emplace_back(fea_val);
        hash_id = this->fea_hash.FeaRegister(slot_name, (float)val);
      } else if (slot_type == 0) {
        /// TODO@202108221439: In case `val` is a float but discrete feature.
        fea_val = std::to_string((int32_t)val);
        libsvm_val.emplace_back("1");
        hash_id = this->fea_hash.FeaRegister(slot_name, fea_val);
      }
    } else if (val.is_string()) {
      if (slot_type == 0) {
        fea_val = val;
        libsvm_val.emplace_back("1"); 
        hash_id = this->fea_hash.FeaRegister(slot_name, fea_val);
      } else if (slot_type == 1) {
        fea_val = std::to_string(std::stof((std::string)val));
        libsvm_val.emplace_back(fea_val);
        hash_id = this->fea_hash.FeaRegister(slot_name, (float)val);
      }
    } else if (val.is_array()) {
      if (val.size() > 0) {
        std::vector<float> fea_val_vec = val.get< std::vector<float> >();
        hash_id = this->fea_hash.FeaRegister(slot_name, fea_val_vec);
        libsvm_val.resize(hash_id.size());
        for (int32_t j = 0; j < libsvm_val.size(); ++j) {
          libsvm_val[j] = std::to_string(fea_val_vec[j]);
        }
      }
    }
    for (int32_t i = 0; i < hash_id.size(); ++i) {
      output += (std::to_string(hash_id[i]) + ":" + libsvm_val[i] + " ");
    }
  }
  if (flat_json.find(this->label) != flat_json.end()) {
    label_str = "error";
    if (flat_json[this->label].is_string()) { label_str = flat_json[this->label]; }
    if (flat_json[this->label].is_number()) {
      label_str = std::to_string((float)flat_json[this->label]);
    }
  }
  output = label_str + " " + output;
  return output;
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

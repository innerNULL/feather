/// file: LibsvmExtractor.cpp


#include <iostream>
#include <spdlog/spdlog.h>

#include "feather/LibsvmExtractor.h"


namespace feather {


LibsvmExtractor::LibsvmExtractor(
    const std::string& feahash_conf, const bool compress) {
  if (compress) {
    /// TODO@202108201502
    spdlog::warn("Now `LibsvmExtractor` not support 'compress' mode...");
  } else {
    this->fea_hash = FeaHash(feahash_conf);
  }
}


std::string LibsvmExtractor::Extract(const nlohmann::json& flat_json) {
  std::string output;
  for (auto it = flat_json.begin(); it != flat_json.end(); ++it) {
    //std::cout << it.key() << " : " << it.value() << std::endl;
    std::string fea_val;
    auto val = it.value();
    if (it.value().is_number()) {
      fea_val = std::to_string((float)val);
    } else if (it.value().is_string()) {
      fea_val = it.value();
    }
    int64_t hash_id = this->fea_hash.FeaRegister(it.key(), fea_val);
    output += (std::to_string(hash_id) + ":1 ");
  }
  return output;
}


std::string LibsvmExtractor::Extract(const std::string& flat_json) {
  nlohmann::json flat_json_obj = nlohmann::json::parse(flat_json);
  return this->Extract(flat_json_obj);
}


void LibsvmExtractor_pybind(py::module &m) {
  py::class_<LibsvmExtractor>(m, "LibsvmExtractor")
      .def(py::init<>())
      .def(py::init<const std::string&, const bool>())
      .def("Extract", 
          static_cast<std::string (LibsvmExtractor::*)(const std::string&)>(
            &LibsvmExtractor::Extract));
}


} // namespace feather

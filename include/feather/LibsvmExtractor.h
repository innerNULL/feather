/// file: LibsvmExtractor.h


#ifndef FEATHER_LIBSVMEXTRACTOR_H_
#define FEATHER_LIBSVMEXTRACTOR_H_


#include <string>
#include <unordered_map>
#include <pybind11/pybind11.h>
#include <nlohmann/json.hpp>

#include "feather/FeaExtractor.h"
#include "feather/FeaHash.h"

namespace py = pybind11;


namespace feather {


class LibsvmExtractor : public FeaExtractor {
 public:
  LibsvmExtractor() = default;
  LibsvmExtractor(const std::string& feahash_conf, 
      const std::string& label="label", const bool compress=false);

  int32_t CheckField(const nlohmann::json& flat_json, const std::string& field);

  std::string Extract(const std::string& flat_json);
  std::string Extract(const nlohmann::json& flat_json);

 protected:
  //FeaValue* JsonVal2FeaVal(
  //    const int8_t type, const nlohmann::basic_json& val);

 private:
  std::string label;
  FeaHash fea_hash;
  bool compress = false;
  std::unordered_map<int64_t, int64_t> hash_id2libsvm_id;
  std::unordered_map<int64_t, int64_t> libsvm_id2hash_id;
};


void LibsvmExtractor_pybind(py::module &m); 


} // namespace feather


#endif

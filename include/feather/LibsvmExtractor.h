/// file: LibsvmExtractor.h


#ifndef FEATHER_LIBSVMEXTRACTOR_H_
#define FEATHER_LIBSVMEXTRACTOR_H_


#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "feather/FeaExtractor.h"
#include "feather/FeaHash.h"


namespace feather {


class LibsvmExtractor : public FeaExtractor {
 public:
  LibsvmExtractor() = default;
  LibsvmExtractor(const std::string& feahash_conf, 
      const std::string& label="label", const bool index=false);

  int32_t CheckField(const nlohmann::json& flat_json, const std::string& field);

  std::string Extract(const std::string& flat_json, const bool with_label=true);
  std::string Extract(const nlohmann::json& flat_json, const bool with_label=true);
  
  std::vector<std::string> BatchExtract(
      const std::vector<nlohmann::json>& flat_json, const bool with_label=true);
  std::vector<std::string> BatchExtract(
      const std::vector<std::string>& flat_json, const bool with_label=true);

  std::string ExtractLabel(const nlohmann::json& flat_json);

 protected:
  FeaValue* JsonVal2FeaVal(
      const int8_t type, const nlohmann::json& json_val);

 private:
  std::string label;
  FeaHash fea_hash; 
  bool index = false; /// If convert fea-hash to index by rerank.
  std::unordered_map<int64_t, int64_t> hash_id2libsvm_id;
  std::unordered_map<int64_t, int64_t> libsvm_id2hash_id;
};


} // namespace feather


#endif

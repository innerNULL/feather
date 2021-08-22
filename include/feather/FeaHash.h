/// file: core.h


#ifndef FEATHER_FEAHASH_H_
#define FEATHER_FEAHASH_H_


#include <functional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>

#include "feather/FeaSlot.h"

namespace py = pybind11;


namespace feather {


class FeaHash {
 public:
  FeaHash();
  FeaHash(const std::string& conf_path);
 
  int32_t SlotRegister(const std::string& fea_name, const int32_t slot_id, 
      const int32_t bucket_size, const int16_t slot_type);

  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const std::string& fea_value);
  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const std::vector<float>& fea_value);
  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const float fea_value);

  const nlohmann::json& GetMeta();

  void Merge(FeaHash fea_hash);

 protected:
  /**
   * @brief Only works for discrete feature.
   */
  std::vector<int64_t> Fea2FeaID(const std::string& name, const std::string& val);
  std::string BucketID2BucketCode(const int32_t bucket_id);
 
 private:
  std::string conf_path;
  nlohmann::json fea_hash = {
      { "meta", {} }, 
      { "slots", {} }
  };
  int32_t slot_num = 0;
  int32_t val_hash_digits = 0;

  std::unordered_map<std::string, FeaSlot> name2slot;
  std::unordered_map<int32_t, std::string> slot2name;
  std::unordered_map<std::string, int64_t> fea_val2fea_id;
  std::unordered_map<int64_t, std::string> fea_id2fea_group;
  std::unordered_map< int64_t, std::vector<std::string> > fea_id2fea_vals;
};


void FeaHash_pybind(py::module& m);


} // namespace feather


#endif 

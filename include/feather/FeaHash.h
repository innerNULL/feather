/// file: core.h


#ifndef FEATHER_FEAHASH_H_
#define FEATHER_FEAHASH_H_


#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>

#include "feather/BiDict.h"
#include "feather/FeaSlot.h"
#include "feather/FeaValue.h"

namespace py = pybind11;


namespace feather {


class FeaHash {
 public:
  FeaHash();
  FeaHash(const std::string& conf_path, const bool reversible=true);
 
  int32_t SlotRegister(const std::string& fea_name, const int32_t slot_id, 
      const int32_t bucket_size, const int16_t slot_type);

  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const std::string& fea_value);
  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const int32_t fea_value);
  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const std::vector<float>& fea_value);
  std::vector<int64_t> FeaRegister(
      const std::string& fea_name, const float fea_value);

  std::string FeaHash2FeaName(const int64_t fea_hash);

  std::string FeaHash2FeaIndex(const int64_t fea_hash);

  //const nlohmann::json& GetMeta();

  const std::unordered_map<std::string, FeaSlot>* GetSlots();

  int16_t FeaValCheck(const std::string& name);
  int16_t FeaValCheck(const std::string& name, FeaValue* val);

  std::vector<int64_t> FeaVal2FeaHash(FeaValue* fea_val, FeaSlot* fea_slot);

  //void Merge(FeaHash fea_hash);

 protected:
  std::vector<int32_t> FeaVal2FeaHashBucket(FeaValue* fea_val, FeaSlot* fea_slot);

  /// for example, bucket id could be 35, bucket code could be '00035'.
  std::vector<std::string> FeaVal2FeaHashBucketCode(FeaValue* fea_val, FeaSlot* fea_slot);

  void Hash2IndexDictBuild(const std::string& conf_path);
 
 private:
  std::string conf_path;
  std::vector<std::string> dict_schema_ = {"fea_name", "slot"};
  std::shared_ptr<BiDict> slot2name_dict_ = 
      std::make_shared<BiDict>(this->dict_schema_);
  std::vector<std::string> hash2index_schema_ = {"fea_hash", "fea_index"};
  std::shared_ptr<BiDict> hash2index_dict_ = 
      std::make_shared<BiDict>(this->hash2index_schema_);
  /*
  nlohmann::json fea_hash = {
      { "meta", {} }, 
      { "slots", {} }
  };
  */
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

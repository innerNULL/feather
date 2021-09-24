/// file: core.h


#ifndef FEATHER_FEAHASH_H_
#define FEATHER_FEAHASH_H_


#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "feather/BiDict.h"
#include "feather/FeaSlot.h"
#include "feather/FeaValue.h"


namespace feather {


class FeaHash {
 public:
  FeaHash();
  FeaHash(const std::string& conf_path, const bool reversible=true);
 
  int32_t SlotRegister(const std::string& fea_name, const int32_t slot_id, 
      const int32_t bucket_size, const int16_t slot_type);


  template<typename VAL_TYPE>
  std::vector<int64_t> Fea2FeaHash(
      const std::string& fea_name, const VAL_TYPE& fea_value);

  std::vector<int64_t> GetFeaHash(
      const std::string& fea_name, const std::string& fea_value);
  std::vector<int64_t> GetFeaHash(
      const std::string& fea_name, const int32_t fea_value);
  std::vector<int64_t> GetFeaHash(
      const std::string& fea_name, const std::vector<float>& fea_value);
  std::vector<int64_t> GetFeaHash(
      const std::string& fea_name, const float fea_value);
  std::vector<int64_t> GetFeaHash(
      const std::string& fea_name, const std::vector<std::string>& fea_value);
  std::vector<int64_t> GetFeaHash(
      const std::string& fea_name, const std::vector<int32_t>& fea_value);

  std::string FeaHash2FeaName(const int64_t fea_hash);

  std::string FeaHash2FeaIndexStr(const int64_t fea_hash);

  //const nlohmann::json& GetMeta();

  const std::unordered_map<std::string, FeaSlot>* GetSlots();

  int16_t FeaValCheck(const std::string& name);
  int16_t FeaValCheck(const std::string& name, FeaValue* val);

  std::vector<int64_t> FeaVal2FeaHash(FeaValue* fea_val, FeaSlot* fea_slot);

  const FeaSlot* GetSlot(const std::string& fea_name);
  const FeaSlot* GetSlot(const int32_t fea_slot); // TODO

  int32_t GetFeaBucketCodeLength();

  void SchemaLineRegister(const std::vector<std::string>& schema_line);

  void Hash2IndexDictBuild();

  //void Transfer();

 protected:
  std::vector<int32_t> FeaVal2FeaHashBucket(FeaValue* fea_val, FeaSlot* fea_slot);

  /// for example, bucket id could be 35, bucket code could be '00035'.
  std::vector<std::string> FeaVal2FeaHashBucketCode(FeaValue* fea_val, FeaSlot* fea_slot);
 
 private:
  std::string conf_path;
  std::vector<std::string> dict_schema_ = {"fea_name", "slot"};
  std::shared_ptr<BiDict> slot2name_dict_ = 
      std::make_shared<BiDict>(this->dict_schema_);
  std::vector<std::string> hash2index_schema_ = {"fea_hash", "fea_index"};
  std::shared_ptr<BiDict> hash2index_dict_ = 
      std::make_shared<BiDict>(this->hash2index_schema_);
  std::unordered_map<std::string, FeaSlot> name2slot;

  int32_t slot_num = 0;
  int32_t slot_digits = -1;
  int32_t bucket_code_digits_ = -1; /// Feature value's hash-bucket code length.
  int32_t fea_index = 1; /// // Feature hash index recorder. 
};


} // namespace feather


#endif 

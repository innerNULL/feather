/// file: FeaSlot.h


#ifndef FEATHER_FEASLOT_H_
#define FEATHER_FEASLOT_H_


#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include <pybind11/pybind11.h>

namespace py = pybind11;


namespace feather {


class FeaSlot {
 public:
  FeaSlot() = default;
  /**
   * @brief
   * For float array, bucket_size MUST equals to array size.
   * For discrete feature such as tags, bucket_size is hash-bucket number.
   * For continuous feature, bucket_size MUST equalt to 1.
   */
  FeaSlot(const std::string& name, const int32_t slot_id, 
      const int32_t bucket_size, const int8_t slot_type=0);

  void Info();
  int32_t ValRegister(const std::string val, const int64_t val_hash);
  int32_t GetBucketID(const std::string& val);
  int32_t GetSlotID();
  int32_t GetBucketSize();
  void Merge(FeaSlot fea_slot);

 private:
  int8_t slot_type; /// 0 for discrete, 1 for continuous, 2 for array.
  std::string kv_delimiter = "#";
  std::string name;
  int32_t slot_id = -1;
  int32_t bucket_size = -1;

  std::unordered_map<std::string, int32_t> val2bucket;
  std::unordered_map<std::string, int32_t> val2last_bucket; /// TODO and EXPT
  std::unordered_map< int32_t, std::unordered_map<std::string, char> > bucket2vals;

  //std::unordered_map<std::string, int64_t> val2fea_id;
  //std::unordered_map<int64_t, std::string> fea_id2vals;
};


void FeaSlot_pybind(py::module &m);


} // namespace feather


#endif
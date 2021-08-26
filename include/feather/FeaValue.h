/// file: FeaValue.h


#ifndef FEATHER_FEAVALUE_H_
#define FEATHER_FEAVALUE_H_


#include <string>
#include <vector>


namespace feather {


class FeaValue {
 public:
  FeaValue(const int32_t val, const int16_t type=0);
  FeaValue(const float& val, const int16_t type=1);
  FeaValue(const std::string& val, const int16_t type=0);
  FeaValue(const std::vector<int32_t>* val);
  FeaValue(const std::vector<std::string>* val);
  FeaValue(const std::vector<float>* val);

  std::vector<int64_t> GetHash();

 private:
  int16_t type = -1;
  std::vector<float>* vec_val = nullptr;
  std::string discrete_val;
  float continuous_val;
};


} // namespace feather


#endif

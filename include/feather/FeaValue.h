/// file: FeaValue.h


#ifndef FEATHER_FEAVALUE_H_
#define FEATHER_FEAVALUE_H_


#include <string>
#include <vector>


namespace feather {


class FeaValue {
 public:
  FeaValue() = default;    
  FeaValue(const int32_t val, const int16_t type=0);
  FeaValue(const float& val, const int16_t type=1);
  FeaValue(const double& val, const int16_t type=1);
  FeaValue(const std::string& val, const int16_t type=0);
  FeaValue(const std::vector<int32_t>* val);
  FeaValue(const std::vector<std::string>* val);
  FeaValue(const std::vector<float>* val);
  FeaValue(const std::vector<double>* val); 
  FeaValue(const std::vector<int32_t>& val);
  FeaValue(const std::vector<std::string>& val);
  FeaValue(const std::vector<float>& val);
  FeaValue(const std::vector<double>& val);

  std::vector<int64_t> GetHash();
 
  const int16_t GetType();

  const std::vector<float>* GetVecValue();
  const float GetContinuous();

 private:
  int16_t type = -1;
  std::vector<float> vec_val;
  std::string discrete_val;
  float continuous_val;
};


} // namespace feather


#endif

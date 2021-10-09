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
  FeaValue(const std::vector<int32_t>* val, const int16_t type);
  FeaValue(const std::vector<std::string>* val, const int16_t type);
  FeaValue(const std::vector<float>* val, const int16_t type);
  FeaValue(const std::vector<double>* val, const int16_t type); 
  FeaValue(const std::vector<int32_t>& val, const int16_t type);
  FeaValue(const std::vector<std::string>& val, const int16_t type);
  FeaValue(const std::vector<float>& val, const int16_t type);
  FeaValue(const std::vector<double>& val, const int16_t type);

  template<typename HASH_VAL=int64_t>
  std::vector<HASH_VAL> GetHash(uint16_t hash_type=0);
  template<typename HASH_VAL=int64_t>
  std::vector<HASH_VAL> GetHash(HASH_VAL (*hash_func)(std::string));

  const int16_t GetType();

  const std::vector<float>* GetVecValue();
  const float GetContinuous();

 private:
  int16_t type = -1;
  std::vector<float> vec_val;
  /// Use `std::vector<std::string>` to support multi-hot mode.
  // std::string discrete_val; 
  std::vector<std::string> discrete_val;
  float continuous_val;
};


} // namespace feather


#endif

/// file: FeaValue.cpp


#include <string>
#include <algorithm>
#include <spdlog/spdlog.h>

#include "./feather/FeaValue.h"


namespace feather {


FeaValue::FeaValue(const int32_t val, const int16_t type) {
  this->type = type;
  if (type == 0) {
    this->discrete_val = std::to_string(val);
  } else if (type == 1) {
    this->continuous_val = (float)val;
  } else {
    spdlog::error("Current feature-value only belongs to \
        descrete or continuous feature.");
  }
}


FeaValue::FeaValue(const float& val, const int16_t type) {
  this->type = type;
  if (type == 0) {
    this->discrete_val = std::to_string(val);
  } else if (type == 1) {
    this->continuous_val = val;
  } else {
    spdlog::error("Current feature-value only belongs to \
        descrete or continuous feature.");
  }
}


FeaValue::FeaValue(const double& val, const int16_t type) {
  new (this) FeaValue((float)val, type);
}


FeaValue::FeaValue(const std::string& val, const int16_t type) {
  this->type = type;
  if (type == 0) {
    this->discrete_val = val;
  } else if (type == 1) {
    this->continuous_val = std::stof(val);
  } else {
    spdlog::error("Current feature-value only belongs to \
        descrete or continuous feature.");
  }
}


FeaValue::FeaValue(const std::vector<int32_t>* val) {
  this->type = 2;
  this->vec_val.resize(val->size());
  for (int32_t i = 0; i < val->size(); ++i) {
    this->vec_val.at(i) = (float)(val->at(i));
  }
}


FeaValue::FeaValue(const std::vector<int32_t>& val) {
  new (this) FeaValue(&val);
}


FeaValue::FeaValue(const std::vector<std::string>* val) {
  this->type = 2;
  this->vec_val.resize(val->size());
  for (int32_t i = 0; i < val->size(); ++i) {
    this->vec_val.at(i) = std::stof(val->at(i));
  }
}


FeaValue::FeaValue(const std::vector<std::string>& val) {
  new (this) FeaValue(&val);  
}


FeaValue::FeaValue(const std::vector<float>* val) {
  this->type = 2;
  this->vec_val.resize(val->size());
  for (int32_t i = 0; i < val->size(); ++i) {
    this->vec_val.at(i) = val->at(i);
  }
}


FeaValue::FeaValue(const std::vector<float>& val) {
  new (this) FeaValue(&val);
}


FeaValue::FeaValue(const std::vector<double>* val) {
  this->type = 2;
  this->vec_val.resize(val->size());
  for (int32_t i = 0; i < val->size(); ++i) {
    this->vec_val.at(i) = (float)(val->at(i));
  }
}


FeaValue::FeaValue(const std::vector<double>& val) {
  new (this) FeaValue(&val);
}


std::vector<int64_t> FeaValue::GetHash() {
  std::vector<int64_t> output;
  if (this->type == 0) {
    output.emplace_back(
        std::hash<std::string>()(this->discrete_val));
  } else if (this->type == 1) {
    /// Since for continuous-feature, the feature-hash bucket-size 
    /// should always be 1, so return hash value as 1 can 
    /// automatically asign single continuously feature value to 
    /// bucket zero, which is also the only bucket for this feature 
    /// slot.
    output.emplace_back(1);
  } else if (this->type == 2) {
    output.resize(this->vec_val.size());
    /// This is because for a (float) vector feature, we limit it 
    /// hash bucket number must be same with its dimension, so each 
    /// element should correponding to 1 bucket in the order from 
    /// 0 to dim, so for element number 0 to `this->vec_val.size()`, 
    /// we assign its hash value as 1, 2, 3, ... , dim + 1, so when 
    /// calculated hash-bucket id by modulo hash value on hash bucket,
    /// each element will correponding bucket 0, 1, 2, ... , dim.
    for (int32_t i = 0; i < this->vec_val.size(); ++i) {
      output[i] = i + this->vec_val.size(); 
    }
  } 
  return output;
}


const std::vector<float>* FeaValue::GetVecValue() {
  return &(this->vec_val);
}


const int16_t FeaValue::GetType() { return this->type; }


} // namespace feather

/// file: FeaValue_test.h


#ifndef FEATHER_TEST_CASES_FEAVALUE_TEST_H_
#define FEATHER_TEST_CASES_FEAVALUE_TEST_H_


#include <sstream>
#include <functional>
#include <string>
#include <gmock/gmock.h>

#include "feather/FeaValue.h"
#include "../common/constants.h"


// Discrete Feature Value.
feather::FeaValue discrete_fea_0("3");
feather::FeaValue discrete_fea_1("3", 0);
feather::FeaValue discrete_fea_2(3);
feather::FeaValue discrete_fea_3(3, 0);
feather::FeaValue discrete_fea_4(3.14, 0);

// Continuous Feature Value.
feather::FeaValue continuous_fea_0("3", 1);
feather::FeaValue continuous_fea_1(3, 1);
feather::FeaValue continuous_fea_2(3.14);
feather::FeaValue continuous_fea_3(3.14, 1);

// Vector Feature Value: Vector/Embedding feature.
/// call to constructor of 'feather::FeaValue' is ambiguous. 
// feather::FeaValue vec_fea_0({1,2,3});
std::vector<int32_t> vec_0 = {1, 2, 3};
feather::FeaValue vec_fea_0(vec_0, 2);
std::vector<float> vec_1 = {1.1, 2.2, 3.3};
feather::FeaValue vec_fea_1(vec_1, 2);
std::vector<std::string> vec_2 = {"1.1", "2.2", "3.3"};
feather::FeaValue vec_fea_2(vec_2, 2);
std::vector<std::string> vec_3 = {"1", "2", "3"};
feather::FeaValue vec_fea_3(vec_3, 2); 

// Vector Feature Value: Categorical multi-hot feature. 
std::vector<int32_t> token_vec_0 = {1, 2, 3};
feather::FeaValue multi_hot_vec_fea_0(token_vec_0, 0);
std::vector<float> token_vec_1 = {1.1, 2.2, 3.3, 4.4};
feather::FeaValue multi_hot_vec_fea_1(token_vec_1, 0);
std::vector<std::string> token_vec_2 = {"1", "2", "3", "4", "5"};
feather::FeaValue multi_hot_vec_fea_2(token_vec_2, 0);


TEST(TEST_FeaValue, FeaValue) {
  // Discrete Feature Value.
  feather::FeaValue discrete_fea_0("3");
  feather::FeaValue discrete_fea_1("3", 0);
  feather::FeaValue discrete_fea_2(3);
  feather::FeaValue discrete_fea_3(3, 0);
  feather::FeaValue discrete_fea_4(3.14, 0);
  
  // Continuous Feature Value.
  feather::FeaValue continuous_fea_0("3", 1);
  feather::FeaValue continuous_fea_1(3, 1);
  feather::FeaValue continuous_fea_2(3.14);
  feather::FeaValue continuous_fea_3(3.14, 1);

  // Vector Feature Value.
  /// call to constructor of 'feather::FeaValue' is ambiguous. 
  // feather::FeaValue vec_fea_0({1,2,3});
  std::vector<int32_t> vec_0 = {1, 2, 3};
  feather::FeaValue vec_fea_0(vec_0, 2);
  std::vector<float> vec_1 = {1.1, 2.2, 3.3};
  feather::FeaValue vec_fea_1(vec_1, 2);
  std::vector<std::string> vec_2 = {"1.1", "2.2", "3.3"};
  feather::FeaValue vec_fea_2(vec_2, 2);
  std::vector<std::string> vec_3 = {"1", "2", "3"};
  feather::FeaValue vec_fea_3(vec_3, 2);

  // Vector Feature Value: Vector/Embedding feature. 
  std::vector<int32_t> token_vec_0 = {1, 2, 3};
  feather::FeaValue multi_hot_vec_fea_0(token_vec_0, 0);
  std::vector<float> token_vec_1 = {1.1, 2.2, 3.3};
  feather::FeaValue multi_hot_vec_fea_1(token_vec_1, 0);
  std::vector<std::string> token_vec_2 = {"1", "2", "3"};
  feather::FeaValue multi_hot_vec_fea_2(token_vec_2, 0);
}


TEST(TEST_FeaValue, GetType) { 
  ASSERT_THAT(discrete_fea_0.GetType(), 0);
  ASSERT_THAT(discrete_fea_1.GetType(), 0);
  ASSERT_THAT(discrete_fea_2.GetType(), 0);
  ASSERT_THAT(discrete_fea_3.GetType(), 0);
  ASSERT_THAT(discrete_fea_4.GetType(), 0);
  ASSERT_THAT(continuous_fea_0.GetType(), 1);
  ASSERT_THAT(continuous_fea_1.GetType(), 1);
  ASSERT_THAT(continuous_fea_2.GetType(), 1);
  ASSERT_THAT(continuous_fea_3.GetType(), 1);
  ASSERT_THAT(vec_fea_0.GetType(), 2);
  ASSERT_THAT(vec_fea_1.GetType(), 2);
  ASSERT_THAT(vec_fea_2.GetType(), 2);
  ASSERT_THAT(vec_fea_3.GetType(), 2);
  ASSERT_THAT(multi_hot_vec_fea_0.GetType(), 0);
  ASSERT_THAT(multi_hot_vec_fea_1.GetType(), 0);
  ASSERT_THAT(multi_hot_vec_fea_2.GetType(), 0);
}


TEST(TEST_FeaValue, GetHash) {
  int64_t discrete_fea_val3_hash = std::hash<std::string>()("3");
  int64_t discrete_fea_val3p14_hash = std::hash<std::string>()(std::to_string(3.14));
  ASSERT_THAT(discrete_fea_0.GetHash()[0], discrete_fea_val3_hash);
  ASSERT_THAT(discrete_fea_1.GetHash()[0], discrete_fea_val3_hash);
  ASSERT_THAT(discrete_fea_2.GetHash()[0], discrete_fea_val3_hash);
  ASSERT_THAT(discrete_fea_3.GetHash()[0], discrete_fea_val3_hash);
  ASSERT_THAT(discrete_fea_4.GetHash()[0], discrete_fea_val3p14_hash);

  ASSERT_THAT(continuous_fea_0.GetHash()[0], 1);
  ASSERT_THAT(continuous_fea_1.GetHash()[0], 1);
  ASSERT_THAT(continuous_fea_2.GetHash()[0], 1);
  ASSERT_THAT(continuous_fea_3.GetHash()[0], 1);

  std::vector<int64_t> vec_0_feahash = {3, 4, 5};
  ASSERT_THAT(vec_fea_0.GetHash(), testing::ContainerEq(vec_0_feahash));
  std::vector<int64_t> vec_1_feahash = {3, 4, 5};
  ASSERT_THAT(vec_fea_1.GetHash(), testing::ContainerEq(vec_1_feahash));
  std::vector<int64_t> vec_2_feahash = {3, 4, 5};
  ASSERT_THAT(vec_fea_2.GetHash(), testing::ContainerEq(vec_2_feahash));
  std::vector<int64_t> vec_3_feahash = {3, 4, 5};
  ASSERT_THAT(vec_fea_3.GetHash(), testing::ContainerEq(vec_3_feahash));

  std::vector<int64_t> multi_hot_fea0_hash = multi_hot_vec_fea_0.GetHash();
  ASSERT_THAT(multi_hot_fea0_hash.size(), token_vec_0.size());
  for (int32_t i = 0; i < token_vec_0.size(); ++i) {
    int64_t target_hash_ = std::hash<std::string>()(std::to_string(token_vec_0[i]));
    ASSERT_THAT(multi_hot_fea0_hash[i], target_hash_);
  }
  std::vector<int64_t> multi_hot_fea1_hash = multi_hot_vec_fea_1.GetHash();
  ASSERT_THAT(multi_hot_fea1_hash.size(), token_vec_1.size());
  for (int32_t i = 0; i < token_vec_1.size(); ++i) {
    std::ostringstream oss_;
    oss_ << token_vec_1[i];
    int64_t target_hash_ = std::hash<std::string>()(oss_.str());
    ASSERT_THAT(multi_hot_fea1_hash[i], target_hash_);
  }
  std::vector<int64_t> multi_hot_fea2_hash = multi_hot_vec_fea_2.GetHash();
  ASSERT_THAT(multi_hot_fea2_hash.size(), token_vec_2.size());
  for (int32_t i = 0; i < token_vec_2.size(); ++i) {
    int64_t target_hash_ = std::hash<std::string>()(token_vec_2[i]);
    ASSERT_THAT(multi_hot_fea2_hash[i], target_hash_);
  }
}


#endif

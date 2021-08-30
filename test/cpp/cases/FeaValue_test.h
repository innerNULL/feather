/// file: FeaValue_test.h


#ifndef FEATHER_TEST_CASES_FEAVALUE_TEST_H_
#define FEATHER_TEST_CASES_FEAVALUE_TEST_H_


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

// Vector Feature Value.
/// call to constructor of 'feather::FeaValue' is ambiguous. 
// feather::FeaValue vec_fea_0({1,2,3});
std::vector<int32_t> vec_0 = {1, 2, 3};
feather::FeaValue vec_fea_0(vec_0);
std::vector<float> vec_1 = {1.1, 2.2, 3.3};
feather::FeaValue vec_fea_1(vec_1);
std::vector<std::string> vec_2 = {"1.1", "2.2", "3.3"};
feather::FeaValue vec_fea_2(vec_2);
std::vector<std::string> vec_3 = {"1", "2", "3"};
feather::FeaValue vec_fea_3(vec_3); 


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
  feather::FeaValue vec_fea_0(vec_0);
  std::vector<float> vec_1 = {1.1, 2.2, 3.3};
  feather::FeaValue vec_fea_1(vec_1);
  std::vector<std::string> vec_2 = {"1.1", "2.2", "3.3"};
  feather::FeaValue vec_fea_2(vec_2);
  std::vector<std::string> vec_3 = {"1", "2", "3"};
  feather::FeaValue vec_fea_3(vec_3); 
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
}

#endif

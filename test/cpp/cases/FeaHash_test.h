/// file: FeaHash_test.h


#ifndef FEATHER_TEST_CASES_FEAHASH_TEST_H_
#define FEATHER_TEST_CASES_FEAHASH_TEST_H_


#include <cmath>
#include <vector>
#include <functional>
#include <string>
#include <gmock/gmock.h>
#include <spdlog/spdlog.h>

#include "feather/FeaHash.h"
#include "../common/constants.h"
#include "../common/test_helper.h"


feather::FeaHash feahash(FEATHER_CONF);


TEST(TEST_FeaHash, FeaHash) {
  feather::FeaHash feahash(FEATHER_CONF);
}


TEST(TEST_FeaHash, SlotRegister) {
  feather::FeaHash feahash_;
  feahash_.SlotRegister("test1", 101, 8, 0);
  feahash_.SlotRegister("test2", 102, 1024, 0);
  feahash_.SlotRegister("test3", 103, 1, 1);
  feahash_.SlotRegister("test4", 104, 128, 2);

  const feather::FeaSlot* slot1 = feahash_.GetSlot("test1");
  const feather::FeaSlot* slot2 = feahash_.GetSlot("test2");
  const feather::FeaSlot* slot3 = feahash_.GetSlot("test3");
  const feather::FeaSlot* slot4 = feahash_.GetSlot("test4");
  const feather::FeaSlot* slot5 = feahash_.GetSlot("test5");

  ASSERT_THAT(slot1->GetSlotID(), 101);
  ASSERT_THAT(slot1->GetBucketSize(), 8);
  ASSERT_THAT(slot1->GetType(), 0);
  ASSERT_THAT(slot1->GetName(), "test1");

  ASSERT_THAT(slot2->GetSlotID(), 102);
  ASSERT_THAT(slot2->GetBucketSize(), 1024);
  ASSERT_THAT(slot2->GetType(), 0);
  ASSERT_THAT(slot2->GetName(), "test2");

  ASSERT_THAT(slot3->GetSlotID(), 103);
  ASSERT_THAT(slot3->GetBucketSize(), 1);
  ASSERT_THAT(slot3->GetType(), 1);
  ASSERT_THAT(slot3->GetName(), "test3");

  ASSERT_THAT(slot4->GetSlotID(), 104);
  ASSERT_THAT(slot4->GetBucketSize(), 128);
  ASSERT_THAT(slot4->GetType(), 2);
  ASSERT_THAT(slot4->GetName(), "test4");

  ASSERT_THAT(slot5, nullptr);
}


TEST(TEST_FeaHash, FeaRegister) {
  /// TODO: Test with demo fea-schema config

  /// Twice test
  feather::FeaHash feahash_;
  feahash_.SlotRegister("test1", 101, 8, 0);
  feahash_.SlotRegister("test2", 102, 1024, 0);
  feahash_.SlotRegister("test3", 103, 1, 1);
  feahash_.SlotRegister("test4", 104, 4, 2);
  int32_t hash_bucket_code_digits = std::string("1024").size();

  std::vector<int64_t> test1_str1_feahash = feahash_.FeaRegister("test1", "1");
  std::vector<int64_t> test1_int1_feahash = feahash_.FeaRegister("test1", 1);
  int64_t test1_str1_hash = std::hash<std::string>()("1");
  ///test1_str1_hash = abs(test1_str1_hash);
  /// TODO The following not same with the result of 
  /// `abs(std::hash<std::string>()("1")) % 8`, wierd...
  ///int32_t test1_str1_feahash_bucket = std::hash<std::string>()("1") % 8;
  int64_t test1_str1_target_hash = feahash_generator(
      test1_str1_hash, 101, 8, hash_bucket_code_digits);
  ASSERT_THAT(test1_str1_feahash.size(), 1);
  ASSERT_THAT(test1_str1_feahash[0], test1_str1_target_hash);
  ASSERT_THAT(test1_int1_feahash[0], test1_str1_target_hash);
  ASSERT_THAT(test1_str1_feahash, test1_int1_feahash);

  std::vector<int64_t> test2_str256_feahash = feahash_.FeaRegister("test2", "256");
  std::vector<int64_t> test2_int256_feahash = feahash_.FeaRegister("test2", 256);
  int64_t test2_str256_hash = std::hash<std::string>()("256");
  int64_t test2_str256_target_hash = feahash_generator(
      test2_str256_hash, 102, 1024, hash_bucket_code_digits);
  ASSERT_THAT(test2_str256_feahash.size(), 1);
  ASSERT_THAT(test2_str256_feahash[0], test2_str256_target_hash);
  ASSERT_THAT(test2_int256_feahash[0], test2_str256_target_hash);
  ASSERT_THAT(test2_str256_feahash, test2_int256_feahash);

  std::vector<int64_t> test3_str3p14_feahash = feahash_.FeaRegister("test3", "3.14");
  //std::vector<int64_t> test3_float3p14_feahash = feahash_.FeaRegister("test3", 3.14);
  std::vector<int64_t> test3_float3p14_feahash = feahash_.FeaRegister(
      "test3", (float)(3.14));
  int64_t test3_str3p14_target_hash = 103 * pow(10, hash_bucket_code_digits);
  ASSERT_THAT(test3_str3p14_feahash.size(), 1);
  ASSERT_THAT(test3_str3p14_feahash[0], test3_str3p14_target_hash);
  ASSERT_THAT(test3_float3p14_feahash[0], test3_str3p14_target_hash);
  ASSERT_THAT(test3_str3p14_feahash, test3_float3p14_feahash);

  std::vector<float> test4_val = {1.1, 2.2, 3.3, 4.4};
  std::vector<int64_t> test4_feahash = feahash_.FeaRegister("test4", test4_val);
  ASSERT_THAT(test4_feahash.size(), test4_val.size());
  for (int32_t i = 0; i < test4_val.size(); ++i) {
    ASSERT_THAT(test4_feahash[i], 104 * pow(10, hash_bucket_code_digits) + i); 
  }
}


#endif

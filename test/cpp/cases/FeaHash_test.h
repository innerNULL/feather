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
  feather::FeaHash feahash_(FEATHER_CONF);
  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), 5);

  /// Fea1 test, discrete-feature, type 0.
  const feather::FeaSlot* slot101 = feahash_.GetSlot("fea1");
  ASSERT_THAT(slot101->GetSlotID(), 101);
  ASSERT_THAT(slot101->GetBucketSize(), 100); 

  std::vector<int64_t> fea1_str128_hash = feahash_.GetFeaHash("fea1", "128");
  std::vector<int64_t> fea1_int128_hash = feahash_.GetFeaHash("fea1", 128);
  int64_t str128_hash = std::hash<std::string>()("128");
  int64_t fea1_str128_target_hash = feahash_generator(
      str128_hash, slot101->GetSlotID(), slot101->GetBucketSize(), 
      feahash_.GetFeaBucketCodeLength());
  ASSERT_THAT(fea1_str128_hash.size(), 1);
  ASSERT_THAT(fea1_str128_hash[0], fea1_str128_target_hash);
  ASSERT_THAT(fea1_int128_hash[0], fea1_str128_target_hash);
  ASSERT_THAT(fea1_str128_hash, fea1_int128_hash);
  /// Fea1 tmp test for dev and check twice
  std::vector<int64_t> fea1_val1_hash = feahash_.GetFeaHash("fea1", "val1");
  std::vector<int64_t> fea1_val2_hash = feahash_.GetFeaHash("fea1", "val2");
  int64_t val1_hash = std::hash<std::string>()("val1");
  int64_t val2_hash = std::hash<std::string>()("val2");
  int64_t target_fea1val1_hash = feahash_generator(
        val1_hash, slot101->GetSlotID(), slot101->GetBucketSize(), 
        feahash_.GetFeaBucketCodeLength());
  int64_t target_fea1val2_hash = feahash_generator(
        val2_hash, slot101->GetSlotID(), slot101->GetBucketSize(), 
        feahash_.GetFeaBucketCodeLength());
  ASSERT_THAT(fea1_val1_hash.size(), 1);
  ASSERT_THAT(fea1_val2_hash.size(), 1);
  ASSERT_THAT(fea1_val1_hash[0], target_fea1val1_hash);
  ASSERT_THAT(fea1_val2_hash[0], target_fea1val2_hash);

  /// Fea10 test, continuous-feature, type 1.
  const feather::FeaSlot* slot110 = feahash_.GetSlot("fea10");
  ASSERT_THAT(slot110->GetSlotID(), 110);
  ASSERT_THAT(slot110->GetBucketSize(), 1); 

  std::vector<int64_t> fea10_float3p14_hash = feahash_.GetFeaHash("fea10", (float)3.14);
  std::vector<int64_t> fea10_str3p14_hash = feahash_.GetFeaHash("fea10", "3.14");
  std::vector<int64_t> fea10_str3p15_hash = feahash_.GetFeaHash("fea10", "3.15"); 
  int64_t target_fea10_hash = 
      slot110->GetSlotID() * pow(10, feahash_.GetFeaBucketCodeLength());
  ASSERT_THAT(fea10_float3p14_hash.size(), 1);
  ASSERT_THAT(fea10_float3p14_hash[0], target_fea10_hash);
  ASSERT_THAT(fea10_float3p14_hash, fea10_str3p14_hash);
  ASSERT_THAT(fea10_float3p14_hash, fea10_str3p15_hash);

  /// Fea11 test, vector-feature, type 2.
  const feather::FeaSlot* slot111 = feahash_.GetSlot("fea11");
  ASSERT_THAT(slot111->GetSlotID(), 111);
  ASSERT_THAT(slot111->GetBucketSize(), 4);
  
  std::vector<float> fea11_vec_1 = {4.0, 3.0, 2.0, 1.0};
  std::vector<int64_t> fea11_vec_1_hash = feahash_.GetFeaHash("fea11", fea11_vec_1);
  ASSERT_THAT(fea11_vec_1_hash.size(), slot111->GetBucketSize());

  for (int32_t i = 0; i < slot111->GetBucketSize(); ++i) {
    int64_t target_feahash = 
        slot111->GetSlotID() * pow(10, feahash_.GetFeaBucketCodeLength()) + i;
    ASSERT_THAT(fea11_vec_1_hash[i], target_feahash);
  }
}


TEST(TEST_FeaHash, GetFeaBucketCodeLength) {
  feather::FeaHash feahash_;
  feahash_.SlotRegister("test1", 101, 8, 0);
  feahash_.SlotRegister("test2", 102, 1024, 0);
  feahash_.SlotRegister("test3", 103, 1, 1);
  feahash_.SlotRegister("test4", 104, 128, 2);
  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), 4);
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


TEST(TEST_FeaHash, GetFeaHash) {
  /// Test with demo fea-schema config: See `TEST(TEST_FeaHash, FeaHash)`

  /// Twice test
  feather::FeaHash feahash_;
  feahash_.SlotRegister("test1", 101, 8, 0);
  feahash_.SlotRegister("test2", 102, 1024, 0);
  feahash_.SlotRegister("test3", 103, 1, 1);
  feahash_.SlotRegister("test4", 104, 4, 2);
  int32_t hash_bucket_code_digits = std::string("1024").size();

  /// Test single discrete feature
  std::vector<int64_t> test1_str1_feahash = feahash_.GetFeaHash("test1", "1");
  std::vector<int64_t> test1_int1_feahash = feahash_.GetFeaHash("test1", 1);
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

  std::vector<int64_t> test2_str256_feahash = feahash_.GetFeaHash("test2", "256");
  std::vector<int64_t> test2_int256_feahash = feahash_.GetFeaHash("test2", 256);
  int64_t test2_str256_hash = std::hash<std::string>()("256");
  int64_t test2_str256_target_hash = feahash_generator(
      test2_str256_hash, 102, 1024, hash_bucket_code_digits);
  ASSERT_THAT(test2_str256_feahash.size(), 1);
  ASSERT_THAT(test2_str256_feahash[0], test2_str256_target_hash);
  ASSERT_THAT(test2_int256_feahash[0], test2_str256_target_hash);
  ASSERT_THAT(test2_str256_feahash, test2_int256_feahash);

  /// Test categorical multi-hot discrete feature
  std::vector<std::string> test1_multi_hot_str = {"1", "2"};
  std::vector<int32_t> test1_multi_hot_int = {1, 2};
  std::vector<int64_t> test1_multi_hot_str_feahash = 
      feahash_.GetFeaHash("test1", test1_multi_hot_str);
  std::vector<int64_t> test1_multi_hot_int_feahash = 
      feahash_.GetFeaHash("test1", test1_multi_hot_int);
  ASSERT_THAT(test1_multi_hot_str_feahash.size(), test1_multi_hot_int.size());
  ASSERT_THAT(test1_multi_hot_int_feahash.size(), test1_multi_hot_int.size());
  for (int32_t i = 0; i < test1_multi_hot_str.size(); ++i) {
    ASSERT_THAT(test1_multi_hot_str_feahash[i], test1_multi_hot_int_feahash[i]);
    ASSERT_THAT(test1_multi_hot_str_feahash[i], 
        feahash_.GetFeaHash("test1", test1_multi_hot_str[i])[0]);
  }

  /// Test continuous feature
  std::vector<int64_t> test3_str3p14_feahash = feahash_.GetFeaHash("test3", "3.14");
  //std::vector<int64_t> test3_float3p14_feahash = feahash_.GetFeaHash("test3", 3.14);
  std::vector<int64_t> test3_float3p14_feahash = feahash_.GetFeaHash(
      "test3", (float)(3.14));
  int64_t test3_str3p14_target_hash = 103 * pow(10, hash_bucket_code_digits);
  ASSERT_THAT(test3_str3p14_feahash.size(), 1);
  ASSERT_THAT(test3_str3p14_feahash[0], test3_str3p14_target_hash);
  ASSERT_THAT(test3_float3p14_feahash[0], test3_str3p14_target_hash);
  ASSERT_THAT(test3_str3p14_feahash, test3_float3p14_feahash);

  /// Test float-vector(embedding) feature.
  std::vector<float> test4_val = {1.1, 2.2, 3.3, 4.4};
  std::vector<int64_t> test4_feahash = feahash_.GetFeaHash("test4", test4_val);
  ASSERT_THAT(test4_feahash.size(), test4_val.size());
  for (int32_t i = 0; i < test4_val.size(); ++i) {
    ASSERT_THAT(test4_feahash[i], 104 * pow(10, hash_bucket_code_digits) + i); 
  }
}


TEST(TEST_FeaHash, SchemaLineRegister) {
  feather::FeaHash feahash_;
  std::vector<std::string> test1_schema = {"test1", "100", "5", "0"};
  std::vector<std::string> test2_schema = {"test2", "101", "128", "2"};
  std::vector<std::string> test3_schema = {"test3", "102", "1", "1"};
  std::vector<std::string> test4_schema = {"test4", "103", "1024", "0"}; 

  feahash_.SchemaLineRegister(test1_schema);
  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), test1_schema[2].size());
  ASSERT_THAT(feahash_.GetSlot("test1")->GetSlotID(), std::stoi(test1_schema[1]));
  ASSERT_THAT(feahash_.GetSlot("test1")->GetBucketSize(), std::stoi(test1_schema[2]));

  feahash_.SchemaLineRegister(test2_schema);
  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), test2_schema[2].size());
  ASSERT_THAT(feahash_.GetSlot("test2")->GetBucketSize(), std::stoi(test2_schema[2]));

  feahash_.SchemaLineRegister(test3_schema);
  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), test2_schema[2].size());

  feahash_.SchemaLineRegister(test4_schema);
  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), test4_schema[2].size());
}


TEST(TEST_FeaHash, FeaHash2FeaIndexStr) {
  feather::FeaHash feahash_;
  std::vector<std::string> test1_schema = {"test1", "100", "5", "0"};
  std::vector<std::string> test2_schema = {"test2", "101", "128", "2"};
  std::vector<std::string> test3_schema = {"test3", "102", "1", "1"};
  std::vector<std::string> test4_schema = {"test4", "103", "1024", "0"}; 
  feahash_.SchemaLineRegister(test2_schema);
  feahash_.SchemaLineRegister(test4_schema);
  feahash_.SchemaLineRegister(test3_schema);
  feahash_.SchemaLineRegister(test1_schema);
  feahash_.Hash2IndexDictBuild();

  ASSERT_THAT(feahash_.GetFeaBucketCodeLength(), 4);
  
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1000000), "1");
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1000004), "5");
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1010000), std::to_string(5 + 1));
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1010031), std::to_string(5 + 32));  
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1010127), std::to_string(5 + 128));
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1020000), std::to_string(5 + 128 + 1));
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1030000), std::to_string(5 + 128 + 1 + 1));
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1030001), std::to_string(5 + 128 + 1 + 2));
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1030511), std::to_string(5 + 128 + 1 + 512));
  ASSERT_THAT(feahash_.FeaHash2FeaIndexStr(1031023), std::to_string(5 + 128 + 1 + 1024));
}


#endif

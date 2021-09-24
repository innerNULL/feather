/// file: LibsvmExtractor_test.h


#ifndef FEATHER_TEST_CASES_LIBSVMEXTRACTOR_H_
#define FEATHER_TEST_CASES_LIBSVMEXTRACTOR_H_


#include <map>
#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <absl/strings/str_split.h>

#include "feather/LibsvmExtractor.h"
#include "../common/constants.h"



TEST(TEST_LibsvmExtractor, LibsvmExtractor) {
  feather::LibsvmExtractor libsvm_extractor_0_ = feather::LibsvmExtractor(
      FEATHER_CONF, "ctr", false);
  feather::LibsvmExtractor libsvm_extractor_1_ = feather::LibsvmExtractor(
      FEATHER_CONF);
  feather::LibsvmExtractor libsvm_extractor_2_ = feather::LibsvmExtractor(
      FEATHER_CONF, "ctr", true);
  feather::LibsvmExtractor libsvm_extractor_3_ = feather::LibsvmExtractor();
}


TEST(TEST_LibsvmExtractor, ExtractLabel) {
  feather::LibsvmExtractor libsvm_extractor_ = feather::LibsvmExtractor(
      FEATHER_CONF, "ctr", false);
  
  nlohmann::json record0 = {
    {"ctr", "0"}, {"fea0", "1"}, {"fea1", 2}, {"fea2", {1, 0, 2}}
  };
  std::string label0 = libsvm_extractor_.ExtractLabel(record0);
  ASSERT_THAT(label0, "0");

  nlohmann::json record1 = {
    {"label", "0"}, {"fea0", "1"}, {"fea1", 2}, {"fea2", {1, 0, 2}}
  };
  std::string label1 = libsvm_extractor_.ExtractLabel(record1);
  ASSERT_THAT(label1, "null");

  nlohmann::json record2 = {
    {"ctr", 0}, {"fea0", "1"}, {"fea1", 2}, {"fea2", {1, 0, 2}}
  };
  std::string label2 = libsvm_extractor_.ExtractLabel(record2);
  ASSERT_THAT(label2, "0");

  nlohmann::json record3 = {
    {"ctr", 0.8}, {"fea0", "1"}, {"fea1", 2}, {"fea2", {1, 0, 2}}
  };
  std::string label3 = libsvm_extractor_.ExtractLabel(record3);
  ASSERT_THAT(std::stof(label3), std::stof("0.8"));
}


TEST(TEST_LibsvmExtractor, Extract) {
  feather::FeaHash feahash_ = feather::FeaHash(FEATHER_CONF);
  feather::LibsvmExtractor extractor0_ = feather::LibsvmExtractor(
      FEATHER_CONF, "ctr", true);
  feather::LibsvmExtractor extractor1_ = feather::LibsvmExtractor(
      FEATHER_CONF, "ctr", false);

  nlohmann::json record0 = {
      {"ctr", "0"}, {"fea_fake", "1"}, {"fea1", 2}, {"fea8", {1, 0, 2}}, 
      {"fea10", 3.14}, {"fea11", {4, 3, 2, 1}}
  };
  nlohmann::json record1 = {
      {"ctr", "0"}, {"fea1", 2}, {"fea8", {1, 0, 2}}, 
      {"fea10", 3.14}, {"fea11", {4, 3, 2, 1}} 
  };
  int32_t index;

  /// Case 1
  std::string record0_libsvm0 = extractor0_.Extract(record0, true);
  index = 0;
  for (const absl::string_view item : absl::StrSplit(record0_libsvm0, " ")) {
    std::string item_(item);
    //std::cout << item_ << std::endl;
    if (item_.size() == 0) { continue; }
    if (index == 0) { ASSERT_THAT(item_, "0"); }
    if (index == 1) { 
      std::string target_ = feahash_.FeaHash2FeaIndexStr(feahash_.GetFeaHash("fea1", 2)[0]) + ":1";
      ASSERT_THAT(item_, target_);
    }
    if (index >= 2 && index < 5) {
      std::vector<int32_t> fea8_val_ = record0["fea8"].get<std::vector<int32_t>>();
      std::vector<std::string> targets_;
      //std::cout << "dbg0 " << item_ << std::endl;
      for (auto val_ : fea8_val_) {
        int64_t curr_feahash_ = feahash_.GetFeaHash("fea8", val_)[0];
        std::string curr_target_ = feahash_.FeaHash2FeaIndexStr(curr_feahash_) + ":1"; 
        targets_.emplace_back(curr_target_);
        //std::cout << "dbg1 " << val_ << ", " << curr_feahash_ << ", " << curr_target_ << std::endl;
      }
      ASSERT_THAT((std::find(targets_.begin(), targets_.end(), item_) != targets_.end()), true);
    }
    if (index == 5) {
      std::string target_ = feahash_.FeaHash2FeaIndexStr(feahash_.GetFeaHash("fea10", (float)3.14)[0]) 
          + ":" + std::to_string(3.14);
      ASSERT_THAT(item_, target_);
    }
    if (index >=6 && index < 10) {
      std::vector<float> fea11_val_ = record0["fea11"].get<std::vector<float>>();
      std::vector<int64_t> fea11_hash_ = feahash_.GetFeaHash("fea11", fea11_val_);
      std::string fea11_index_ = feahash_.FeaHash2FeaIndexStr(fea11_hash_[index - 6]);
      std::string target_ = fea11_index_ + ":" + std::to_string(fea11_val_[index - 6]);
      ASSERT_THAT(item_, target_); 
    }
    index++;
  }
  /// One for label, one for 'fea1', 3 for 'fea8', 1 for 'fea10', 4 for 'fea11'.
  ASSERT_THAT(index, 10);

  /// Case 2
  std::string record0_libsvm1 = extractor1_.Extract(record0, true);
  
  /// Case 3
  std::string record1_libsvm1 = extractor1_.Extract(record1, true);
  ASSERT_THAT(record1_libsvm1, record0_libsvm1);

  /// Case 4
  std::string record0_libsvm1_nolabel = extractor1_.Extract(record0, false);
  index = 0;
  for (const absl::string_view item : absl::StrSplit(record0_libsvm1_nolabel, " ")) {
    if (item.size() == 0) { continue; }
    index++;
  } 
  ASSERT_THAT(index, 10 - 1);
}


TEST(TEST_LibsvmExtractor, BatchExtract) {
  feather::LibsvmExtractor extractor0_ = feather::LibsvmExtractor(
      FEATHER_CONF, "ctr", true);

  nlohmann::json record0 = {
      {"ctr", "0"}, {"fea_fake", "1"}, {"fea1", 2}, {"fea8", {1, 0, 2}}, 
      {"fea10", 3.14}, {"fea11", {4, 3, 2, 1}}
  };
  nlohmann::json record1 = {
      {"ctr", "1"}, {"fea1", "3"}, {"fea8", {5, 1, 3}}, 
      {"fea10", 10.24}, {"fea11", {1, 2, 3, 4}} 
  };
  nlohmann::json record2 = {
      {"ctr", "0"}, {"fea1", 5}, {"fea8", {6, 2, 5}}, 
      {"fea10", 5.12}, {"fea11", {0.8, 1.6, 3.2, 6.4}} 
  };
  std::vector<nlohmann::json> records0 = { record0, record1, record2 };
  std::vector<std::string> records0_output = extractor0_.BatchExtract(records0, true);
  ASSERT_THAT(records0_output.size(), records0.size());
  for (int32_t i = 0;  i < records0_output.size(); ++i) {
    ASSERT_EQ(records0_output[i], extractor0_.Extract(records0[i]));
  }
}


#endif

/// file: dev_test.cpp


#include <iostream>
#include <string>
#include <vector>

#include "feather/FeaSlot.h"
#include "feather/FeaHash.h"
#include "feather/FeaExtractor.h"
#include "feather/LibsvmExtractor.h"
#include "feather/FeaValue.h"
#include "feather/BiDict.h"
#include "feather/utils.h"


void feavalue_example() {
  feather::FeaValue discrete_fea_val0("3");
  feather::FeaValue discrete_fea_val1(3, 0);
  feather::FeaValue discrete_fea_val2(3.14, 0);
  printf("Discrete FeaValue hash of '3': %lld\n", discrete_fea_val0.GetHash()[0]);
  printf("Discrete FeaValue hash of '3': %lld\n", discrete_fea_val1.GetHash()[0]);
  
  feather::FeaValue continuous_fea_val0("6", 1);
  feather::FeaValue continuous_fea_val1(6, 1);
  feather::FeaValue continuous_fea_val2(3.14);
  printf("Continuous FeaValue hash of '6' is %lld\n", continuous_fea_val0.GetHash()[0]);

  feather::FeaValue vec_fea_val0({"1.1", "2.2", "3.3"}, 2);
  std::vector<int64_t> vec_fea_val0_hash = vec_fea_val0.GetHash();
  printf("Vector FeaValue hash of {'1.1', '2.2', '3.3'} is: ");
  for (auto x : vec_fea_val0_hash) { printf("%lld ", x); }
  printf("\n");
  /// feather::FeaValue vec_fea_val1({10, 9, 8}); // Ambitious error
  std::vector<int32_t> vec_fea_val1_raw = {10, 9, 8};
  feather::FeaValue vec_fea_val1(vec_fea_val1_raw, 2);
  std::vector<int64_t> vec_fea_val1_hash = vec_fea_val1.GetHash();
  printf("Vector FeaValue hash of {10, 9, 8} is: ");
  for (auto x : vec_fea_val1_hash) { printf("%lld ", x); }
  printf("\n");
  /// feather::FeaValue vec_fea_val2({1.1, 2.2, 3.3}); // Ambitious error
  std::vector<float> vec_fea_val2_raw = {1.1f, 2.2f, 3.3f};
  feather::FeaValue vec_fea_val2(vec_fea_val2_raw, 2);
  std::vector<int64_t> vec_fea_val2_hash = vec_fea_val2.GetHash();
  printf("Vector FeaValue hash of {1.1, 2.2, 3.3} is: ");
  for (auto x : vec_fea_val2_hash) { printf("%lld ", x); }
  printf("\n");

  printf("end...\n\n");
}


void feahash_example() {
  feather::FeaHash feahash("../conf/feather.conf");
  printf("end...\n\n"); 
}


void utils_example() {
  std::string bucket_code1 = feather::num2str_code(1, 3);
  std::cout << "1 to code " << bucket_code1 << std::endl;
  std::string bucket_code2 = feather::num2str_code(56, 3);
  std::cout << "56 to code " << bucket_code2 << std::endl;
  std::string bucket_code3 = feather::num2str_code(888, 3);
  std::cout << "888 to code " << bucket_code3 << std::endl;
  printf("end...\n\n"); 
}


void bidict_example() {
  feather::BiDict bi_dict({"fea_name", "fea_hash", "slot_id"});
  bi_dict.Register({"fea1", "10100000", "101"});
  /// Map test
  std::string slot_id2fea_name = bi_dict.Map(
      "slot_id", "fea_name", "101")[0];
  printf("%s\n", ("Map slot_id 101 to fea_name " + slot_id2fea_name).c_str());
  
  bi_dict.Persistence();
  printf("end...\n\n"); 
}


int main(int argc, char** argv) {
  feavalue_example();
  feahash_example();
  utils_example();
  bidict_example();
  return 0;
}

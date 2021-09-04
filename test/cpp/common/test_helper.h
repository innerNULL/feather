/// file: test_helper.h


#ifndef FEATHER_TEST_COMMON_TEST_HELPER_H_
#define FEATHER_TEST_COMMON_TEST_HELPER_H_


#include <cmath>


int64_t feahash_generator(
    int64_t val_hash, int32_t slot_id,  
    int32_t bucket_size, int32_t bucket_code_digits) {
  int32_t bucket_id = llabs(val_hash) % bucket_size;
  int64_t fea_hash = (int64_t)slot_id * pow(10, bucket_code_digits) 
      + bucket_id;
  return fea_hash;
}


#endif

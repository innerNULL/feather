/// file: FeaSlot.cpp


#include <cmath>   
#include <spdlog/spdlog.h>

#include "feather/FeaSlot.h"


namespace feather {


FeaSlot::FeaSlot(const FeaSlot& copy) {
  new (this) FeaSlot(
      copy.GetName(), copy.GetSlotID(), copy.GetBucketSize(), 
      copy.GetType());
}


FeaSlot::FeaSlot(const std::string& name, const int32_t slot_id, 
    const int32_t bucket_size, const int8_t slot_type) {
  if (slot_type == 1 && bucket_size != 1) {
    spdlog::error(
        "The bucket-size of type 1 slot (continuous feature slot) must be 1.");
  }
  this->name = name;
  this->slot_id = slot_id;
  this->bucket_size = bucket_size;
  this->slot_type = slot_type;
} 


void FeaSlot::Info() {
  printf("SlotFea:\n");
  printf("\tname: %s\n", this->name.c_str());
  printf("\tslot_id: %i\n", this->slot_id);
  printf("\tbucket_size: %i\n", this->bucket_size);
  printf("\tslot_type: %i\n", this->slot_type);
}


std::string FeaSlot::GetName() const {
  return this->name;
}


int32_t FeaSlot::ValRegister(
    const std::string val, const int64_t val_hash) {
  int32_t bucket_id = -1;
  if (this->slot_type == 0) {
    bucket_id = abs(val_hash % this->bucket_size);

    if (this->val2bucket.find(val) == this->val2bucket.end()) {
      this->val2bucket.emplace(val, bucket_id);
    } else if (this->val2bucket[val] != bucket_id) {
      this->val2last_bucket.emplace(val, this->val2bucket[val]);
      this->val2bucket.emplace(val, bucket_id);
    }
  } else {
    spdlog::debug(
        "Continuous or Array feature '{0}' do not needs registeration.", 
        this->name);
  } 
  return bucket_id;
}


std::vector<int32_t> FeaSlot::GetBucketID(const std::string& val) {
  if (this->slot_type != 0) {
    spdlog::warn(
        "Continuous or Array feature '{0}' doesn't have bucket-id.", 
        this->name);
    return {-1};
  } else {
    return {this->val2bucket[val]};
  }
}


int32_t FeaSlot::GetSlotID() const { return this->slot_id; } 

int8_t FeaSlot::GetType() const { return this->slot_type; } 

int32_t FeaSlot::GetBucketSize() const { return this->bucket_size; }


} // namespace feather

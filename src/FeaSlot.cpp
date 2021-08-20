/// file: FeaSlot.cpp


#include <cmath>   

#include "feather/FeaSlot.h"


namespace feather {


FeaSlot::FeaSlot(const std::string& name, 
    const int32_t slot_id, const int32_t bucket_size) {
  this->name = name;
  this->slot_id = slot_id;
  this->bucket_size = bucket_size;
} 


void FeaSlot::Info() {
  printf("SlotFea:\n");
  printf("\tname: %s\n", this->name.c_str());
  printf("\tslot_id: %i\n", this->slot_id);
  printf("\tbucket_size: %i\n", this->bucket_size);
}


int32_t FeaSlot::ValRegister(
    const std::string val, const int64_t val_hash) {
  int32_t bucket_id = abs(val_hash % this->bucket_size);

  if (this->val2bucket.find(val) == this->val2bucket.end()) {
    this->val2bucket.emplace(val, bucket_id);
  } else if (this->val2bucket[val] != bucket_id) {
    this->val2last_bucket.emplace(val, this->val2bucket[val]);
    this->val2bucket.emplace(val, bucket_id);
  }
  return 0;
}


int32_t FeaSlot::GetBucketID(const std::string& val) {
  return this->val2bucket[val];
}


int32_t FeaSlot::GetSlotID() {
  return this->slot_id;
} 


void FeaSlot_pybind(py::module &m) {
  py::class_<FeaSlot>(m, "FeaSlot")\
      .def(py::init<const std::string&, const int32_t, const int32_t>())
      .def("Info", &FeaSlot::Info)
      .def("ValRegister", &FeaSlot::ValRegister)
      .def("GetBucketID", &FeaSlot::GetBucketID);
}


} // namespace feather

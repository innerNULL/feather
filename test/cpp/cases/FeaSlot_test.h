/// file: FeaSlot_test.h


#ifndef FEATHER_TEST_CASES_FEASLOT_H_
#define FEATHER_TEST_CASES_FEASLOT_H_


#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "feather/FeaSlot.h"
#include "../common/constants.h"


feather::FeaSlot discrete_slot_0 = feather::FeaSlot("fea0", 101, 32);
feather::FeaSlot continuous_slot_0 = feather::FeaSlot("fea1", 102, 1, 1);
feather::FeaSlot vec_slot_0 = feather::FeaSlot("fea2", 103, 4, 2);


TEST(TEST_FeaSlot, FeaSlot) {
  feather::FeaSlot feaslot_0 = feather::FeaSlot();
  feather::FeaSlot* feaslot_1 = new feather::FeaSlot();
  delete feaslot_1;
  
  feather::FeaSlot discrete_slot_0 = feather::FeaSlot("fea0", 101, 32);
  feather::FeaSlot discrete_slot_1 = feather::FeaSlot("fea0", 101, 32, 0);
  feather::FeaSlot continuous_slot_0 = feather::FeaSlot("fea1", 102, 1, 1);
  //feather::FeaSlot continuous_slot_1 = feather::FeaSlot("fea1", 102, 3, 1);
  feather::FeaSlot vec_slot_0 = feather::FeaSlot("fea2", 103, 4, 2);
}


TEST(TEST_FeaSlot, GetSlotID) {
  ASSERT_THAT(discrete_slot_0.GetSlotID(), 101);
  ASSERT_THAT(continuous_slot_0.GetSlotID(), 102);
  ASSERT_THAT(vec_slot_0.GetSlotID(), 103);
}


TEST(TEST_FeaSlot, GetBucketSize) {
  ASSERT_THAT(discrete_slot_0.GetBucketSize(), 32);
  ASSERT_THAT(continuous_slot_0.GetBucketSize(), 1);
  ASSERT_THAT(vec_slot_0.GetBucketSize(), 4);
}


TEST(TEST_FeaSlot, GetType) {
  ASSERT_THAT(discrete_slot_0.GetType(), 0);
  ASSERT_THAT(continuous_slot_0.GetType(), 1);
  ASSERT_THAT(vec_slot_0.GetType(), 2);
}


TEST(TEST_FeaSlot, GetName) {
  ASSERT_THAT(discrete_slot_0.GetName(), "fea0");
  ASSERT_THAT(continuous_slot_0.GetName(), "fea1"); 
  ASSERT_THAT(vec_slot_0.GetName(), "fea2"); 
}


#endif

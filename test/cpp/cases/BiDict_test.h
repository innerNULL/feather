/// file: BiDict_test.h


#ifndef FEATHER_TEST_CASES_BIDICT_H_
#define FEATHER_TEST_CASES_BIDICT_H_


#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "feather/BiDict.h"
#include "../common/constants.h"


feather::BiDict bi_dict({"fea_name", "fea_hash", "slot_id"});


TEST(TEST_BiDict, BiDict) {
  feather::BiDict bi_dict({"fea_name", "fea_hash", "slot_id"});
}


TEST(TEST_BiDict, Map) {
  bi_dict.Register({"fea1", "10100000", "101"});
  bi_dict.Register({"fea1", "10100001", "101"});
  
  ASSERT_THAT(bi_dict.Map("fea_name", "slot_id", "fea1")[0], "101");
  ASSERT_THAT(bi_dict.Map("slot_id", "fea_name", "101")[0], "fea1");
  ASSERT_THAT(bi_dict.Map("fea_hash", "fea_name", "10100000")[0], "fea1");
  ASSERT_THAT(bi_dict.Map("fea_hash", "fea_name", "10100001")[0], "fea1");
  ASSERT_THAT(bi_dict.Map("fea_hash", "slot_id", "10100000")[0], "101");
  ASSERT_THAT(bi_dict.Map("fea_hash", "slot_id", "10100001")[0], "101");
}


#endif

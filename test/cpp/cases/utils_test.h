/// file: utils_test.h


#ifndef FEATHER_TEST_CASES_UTILS_H_
#define FEATHER_TEST_CASES_UTILS_H_


#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "feather/utils.h"
#include "../common/constants.h"


TEST(TEST_utils, num2str_code) {
  int32_t code_digits = 5;
  int32_t int0 = 8;
  int32_t int1 = 16;
  int32_t int2 = 512;
  int32_t int3 = 1024;
  int32_t int4 = 16384;
  ASSERT_THAT(feather::num2str_code<int32_t>(int0, code_digits), "00008");
  ASSERT_THAT(feather::num2str_code<int32_t>(int1, code_digits), "00016");
  ASSERT_THAT(feather::num2str_code<int32_t>(int2, code_digits), "00512");
  ASSERT_THAT(feather::num2str_code<int32_t>(int3, code_digits), "01024");
  ASSERT_THAT(feather::num2str_code<int32_t>(int4, code_digits), "16384");

  /// Sth wrong with float case, but float will not happen in feather scenerio.
}


#endif

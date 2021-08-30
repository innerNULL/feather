/// file: test.cpp


#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "./cases/utils_test.h"
#include "./cases/BiDict_test.h"
#include "./cases/FeaValue_test.h"
#include "./cases/FeaSlot_test.h"


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  return 0;
}

/// file: test.cpp


#include <spdlog/spdlog.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "./cases/utils_test.h"
#include "./cases/BiDict_test.h"
#include "./cases/FeaValue_test.h"
#include "./cases/FeaSlot_test.h"
#include "./cases/FeaHash_test.h"
#include "./cases/LibsvmExtractor_test.h"


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  spdlog::set_level(spdlog::level::off);
  return RUN_ALL_TESTS();
}

#include <gmock/gmock.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
  // Initialize Google Mock (and Google Test) and then run all tests.
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}


#include <vk/vk.h>
#include <gtest/gtest.h>

using namespace vk;

TEST(instance, constructor_does_not_throw) {
  EXPECT_NO_THROW(vk::instance{});
}

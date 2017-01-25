#include <vk/vk.h>
#include <gtest/gtest.h>
#include "device_fixture.h"

using namespace vk;

class image_tests : public device_fixture {
};

TEST_F(image_tests, constructor_does_not_error) {
  extent<3> extent{1024, 1024, 1};
  auto format = texel_format::r8g8b8_srgb;
  vk::image image{*device_, format, extent, 1, 1};
}

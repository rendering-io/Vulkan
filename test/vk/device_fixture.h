#include <vk/vk.h>
#include <gtest/gtest.h>
#include <memory>

class device_fixture : public ::testing::Test {
public:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<vk::instance> instance_;
  std::unique_ptr<vk::device> device_;
};


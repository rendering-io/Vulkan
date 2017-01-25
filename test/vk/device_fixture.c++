#include "device_fixture.h"

void device_fixture::SetUp() {
  instance_ = std::make_unique<vk::instance>();

  const vk::physical_device *best_physical_device = nullptr;
  for (auto &device: instance_->physical_devices()) {
    best_physical_device = &device;
    break;
  }
  
  device_ = std::make_unique<vk::device>(*best_physical_device);
}

void device_fixture::TearDown() {
  device_.reset(nullptr);
  instance_.reset(nullptr);
}


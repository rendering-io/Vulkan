#include <vk/vk.h>

using namespace vk;

class buffer::impl {

};

buffer::buffer(instance)
: impl_{std::make_shared<impl>()} { }


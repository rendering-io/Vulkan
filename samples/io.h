#ifndef SAMPLES_IO_H
#define SAMPLES_IO_H

#include <cstdint>
#include <fstream>
#include <vector>

std::vector<uint32_t> load_shader(const char *path) {
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (in) {
    std::vector<uint32_t> buf;
    in.seekg(0, std::ios::end);

    auto size_in_bytes = in.tellg();
    assert(0 == size_in_bytes % sizeof(uint32_t));
    
    buf.resize(in.tellg() / sizeof(uint32_t));
    in.seekg(0, std::ios::beg);
    in.read(reinterpret_cast<char*>(buf.data()), size_in_bytes);
    in.close();
    return buf;
  }
  throw(errno);
}

#endif // SAMPLES_IO_H


#pragma once
#include "WavData.h"
#include "WavHeader.h"
#include <vector>

namespace Convolve {
class WavFile {
public:
  WavHeader header;
  WavData data;
  std::vector<float> audioData;

  void read(const char *filename);
  void write(const char *filename);
};
} // namespace Convolve
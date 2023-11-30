#pragma once
#include <iostream>
#include <vector>

namespace Convolve {
class Convolver {
public:
  std::vector<float> convolve(const std::vector<float> &x,
                              const std::vector<float> &h);
  void fft(std::vector<float> &data, int nn, int isign);
  void createComplexData(const std::vector<float> &input, float *data);
};
} // namespace Convolve
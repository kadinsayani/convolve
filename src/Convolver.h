#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

namespace Convolve {
class Convolver {
public:
  std::vector<float> convolve(const std::vector<float> &x,
                              const std::vector<float> &h);
  void fft(std::vector<float> &data, int nn, int isign);
};
} // namespace Convolve
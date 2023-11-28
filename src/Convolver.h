#include <iostream>
#include <vector>

namespace Convolve {
class Convolver {
public:
  std::vector<float> convolve(const std::vector<float> &x,
                              const std::vector<float> &h);
};
} // namespace Convolve
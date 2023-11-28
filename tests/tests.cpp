#include "../src/Convolver.h"
#include "../src/WavData.h"
#include "../src/WavFile.h"
#include "../src/WavHeader.h"
#include <catch2/catch_test_macros.hpp>

using namespace Convolve;

TEST_CASE("Convolve Function Test", "[convolve]") {
  Convolver convolver;
  std::vector<float> x = {1.0, 2.0, 3.0};
  std::vector<float> h = {0.5, 0.5};
  std::vector<float> result = convolver.convolve(x, h);
  REQUIRE(result == std::vector<float>{0.5, 1.5, 2.5, 1.5});
}
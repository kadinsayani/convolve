#include "../src/convolve.cpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Convolve Function Test", "[convolve]") {
  std::vector<float> x = {1.0, 2.0, 3.0};
  std::vector<float> h = {0.5, 0.5};
  std::vector<float> result = convolve(x, h);
  REQUIRE(result == std::vector<float>{0.5, 1.5, 2.5, 1.5});
}
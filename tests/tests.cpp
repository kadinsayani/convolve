#include "../src/Convolver.h"
#include "../src/WavData.h"
#include "../src/WavFile.h"
#include "../src/WavHeader.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace Convolve;

TEST_CASE("Convolve function test", "[convolve]") {
  Convolver convolver;
  std::vector<float> x = {1.0, 2.0, 3.0};
  std::vector<float> h = {0.5, 0.5};
  std::vector<float> expected = {0.5, 1.5, 2.5, 1.5};
  std::vector<float> result = convolver.convolve(x, h);
  float epsilon = 1e-5f;
  for (size_t i = 0; i < expected.size(); ++i) {
    REQUIRE(result[i] == Catch::Approx(expected[i]).epsilon(epsilon));
  }
}

TEST_CASE("WavFile read", "[wavfile]") {
  WavFile wavFile;
  REQUIRE_NOTHROW(wavFile.read("guitar_short.wav"));

  SECTION("Check audio data") { REQUIRE(wavFile.audioData.size() > 0); }
}

TEST_CASE("WavFile write", "[wavfile]") {
  WavFile wavFile;
  wavFile.read("guitar_short.wav");
  REQUIRE_NOTHROW(wavFile.write("output.wav"));

  SECTION("Check output file") {
    WavFile outputWavFile;
    outputWavFile.read("output.wav");
    REQUIRE(outputWavFile.audioData.size() > 0);
  }
}

TEST_CASE("Convolver convolve", "[convolver]") {
  Convolver convolver;
  WavFile inputWav, irWav;
  inputWav.read("guitar_short.wav");
  irWav.read("big_hall_mono.wav");

  std::vector<float> outputData;
  REQUIRE_NOTHROW(outputData =
                      convolver.convolve(inputWav.audioData, irWav.audioData));

  SECTION("Check output data") { REQUIRE_FALSE(outputData.empty()); }
}
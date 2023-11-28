#include "WavData.h"

using namespace Convolve;

void WavData::print() {
  std::cout << subchunk2ID << std::endl;
  std::cout << subchunk2Size << std::endl;
}
#pragma once
#include <iostream>

namespace Convolve {
class WavData {
public:
  char subchunk2ID[4];
  int subchunk2Size;

  void print();
};
}
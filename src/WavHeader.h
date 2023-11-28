#pragma once
#include <iostream>

namespace Convolve {
class WavHeader {
public:
  char chunkID[4];
  int chunkSize;
  char format[4];
  char subchunk1ID[4];
  int subchunk1Size;
  short audioFormat;
  short numChannels;
  int sampleRate;
  int byteRate;
  short blockAlign;
  short bitsPerSample;

  void print();
};
}
#include "WavHeader.h"

using namespace Convolve;

void WavHeader::print() {
  std::cout << chunkID << std::endl;
  std::cout << chunkSize << std::endl;
  std::cout << format << std::endl;
  std::cout << subchunk1ID << std::endl;
  std::cout << subchunk1Size << std::endl;
  std::cout << audioFormat << std::endl;
  std::cout << numChannels << std::endl;
  std::cout << sampleRate << std::endl;
  std::cout << byteRate << std::endl;
  std::cout << blockAlign << std::endl;
  std::cout << bitsPerSample << std::endl;
}
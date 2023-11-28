#include "Convolver.h"
#include "WavFile.h"
#include <iostream>
#include <vector>

using namespace Convolve;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: convolve inputfile IRfile outputfile" << std::endl;
    return 1;
  }

  WavFile inputWav, irWav, outputWav;
  Convolver convolver;

  inputWav.read(argv[1]);
  irWav.read(argv[2]);

  if (inputWav.audioData.empty() || irWav.audioData.empty()) {
    std::cerr << "Error: inputfile or impulse response is empty" << std::endl;
    return 1;
  }

  outputWav.audioData = convolver.convolve(inputWav.audioData, irWav.audioData);
  outputWav.write(argv[3]);

  return 0;
}

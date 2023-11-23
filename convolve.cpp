#include <fstream>
#include <iostream>
#include <vector>

struct WAVHeader {
  char chunkID[4];
  uint32_t chunkSize;
  char format[4];
  char subchunk1ID[4];
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  char subchunk2ID[4];
  uint32_t subchunk2Size;
};

std::vector<float> convolve(const std::vector<float> &x,
                            const std::vector<float> &h) {
  std::vector<float> y(x.size() + h.size() - 1, 0.0);
  std::cout << "Convolving input signal and impulse response" << std::endl;

  int n, m;

  for (n = 0; n < y.size(); n++) {
    y[n] = 0.0;
  }

  for (n = 0; n < x.size(); n++) {
    for (m = 0; m < h.size(); m++) {
      y[n + m] += x[n] * h[m];
    }
  }

  return y;
}

std::vector<float> readWAV(const char *filename) {
  std::cout << "Reading WAV file: " << filename << std::endl;
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
  }

  WAVHeader header;
  file.read(reinterpret_cast<char *>(&header), sizeof(WAVHeader));
  std::vector<float> inputSignal(header.subchunk2Size /
                                 (header.bitsPerSample / 8));
  file.read(reinterpret_cast<char *>(inputSignal.data()), header.subchunk2Size);
  float scalingFactor = 1.0f / 32768.0f;
  for (size_t i = 0; i < inputSignal.size(); i++) {
    inputSignal[i] *= scalingFactor;
  }
  file.close();

  return inputSignal;
}

void writeWAV(const char *filename, std::vector<float> outputSignal) {
  std::cout << "Writing output signal to: " << filename << std::endl;
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
  }

  float scalingFactor = 32768.0f;
  std::vector<int16_t> scaledOutput(outputSignal.size());
  for (size_t i = 0; i < outputSignal.size(); i++) {
    scaledOutput[i] = static_cast<int16_t>(outputSignal[i] * scalingFactor);
  }

  WAVHeader header;
  std::strcpy(header.chunkID, "RIFF");
  header.chunkSize =
      sizeof(WAVHeader) + outputSignal.size() * sizeof(float) - 8;
  std::strcpy(header.format, "WAVE");
  std::strcpy(header.subchunk1ID, "fmt ");
  header.subchunk1Size = 16;
  header.audioFormat = 1;
  header.numChannels = 1;
  header.sampleRate = 44100;
  header.bitsPerSample = 16;
  std::strcpy(header.subchunk2ID, "data");
  header.subchunk2Size = outputSignal.size() * sizeof(float);
  file.write(reinterpret_cast<const char *>(&header), sizeof(WAVHeader));
  file.write(reinterpret_cast<const char *>(outputSignal.data()),
             outputSignal.size());
  file.close();
}

void printVector(const char *title, const std::vector<float> &x) {
  for (size_t i = 0; i < x.size(); i++) {
    std::cout << i << "\t\t" << x[i] << std::endl;
  }
}

int main(int argc, char *argv[]) {
  std::vector<float> inputSignal;
  std::vector<float> impulseResponse;
  std::vector<float> outputSignal;

  const char *outputFile = nullptr;

  if (argc == 4) {
    outputFile = argv[3];
  } else {
    std::cerr << "Usage: convolve inputfile IRfile outputfile" << std::endl;
    return 1;
  }

  inputSignal = readWAV(argv[1]);
  impulseResponse = readWAV(argv[2]);

  if (inputSignal.empty() || impulseResponse.empty()) {
    std::cerr << "Error: inputfile or impulse response is empty" << std::endl;
    return 1;
  }

  outputSignal = convolve(inputSignal, impulseResponse);
  writeWAV(argv[3], outputSignal);

  return 0;
}

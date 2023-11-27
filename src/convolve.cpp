#include <iostream>
#include <vector>

typedef struct {
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
} WavHeader;

typedef struct {
  char subchunk2ID[4];
  int subchunk2Size;
} WavData;

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

void printWavHeader(WavHeader header) {
  std::cout << header.chunkID << std::endl;
  std::cout << header.chunkSize << std::endl;
  std::cout << header.format << std::endl;
  std::cout << header.subchunk1ID << std::endl;
  std::cout << header.subchunk1Size << std::endl;
  std::cout << header.audioFormat << std::endl;
  std::cout << header.numChannels << std::endl;
  std::cout << header.sampleRate << std::endl;
  std::cout << header.byteRate << std::endl;
  std::cout << header.blockAlign << std::endl;
  std::cout << header.bitsPerSample << std::endl;
}

void printWavData(WavData data) {
  std::cout << data.subchunk2ID << std::endl;
  std::cout << data.subchunk2Size << std::endl;
}

std::vector<float> readWav(const char *filename) {
  std::cout << "Reading WAV file: " << filename << std::endl;
  FILE *file = fopen(filename, "rb");

  WavHeader header;
  WavData data;

  fread(&header, sizeof(header), 1, file);

  if (header.subchunk1Size != 16) {
    int rem = header.subchunk1Size - 16;
    char nullBytes[rem];
    fread(nullBytes, rem, 1, file);
  }

  printWavHeader(header);

  fread(&data, sizeof(data), 1, file);

  int numSamples = data.subchunk2Size / (header.bitsPerSample / 8);
  size_t dataSize = data.subchunk2Size;
  std::vector<short> audioData(numSamples);
  fread(audioData.data(), sizeof(short), numSamples, file);

  printWavData(data);
  std::cout << "numSamples: " << numSamples << std::endl;

  std::vector<float> inputSignal(numSamples);

  for (size_t i = 0; i < inputSignal.size(); i++) {
    inputSignal[i] = static_cast<float>(audioData[i]) / 32768.0f;
  }

  fclose(file);

  return inputSignal;
}

size_t fwriteIntLSB(int data, FILE *stream) {
  unsigned char array[4];

  array[3] = (unsigned char)((data >> 24) & 0xFF);
  array[2] = (unsigned char)((data >> 16) & 0xFF);
  array[1] = (unsigned char)((data >> 8) & 0xFF);
  array[0] = (unsigned char)(data & 0xFF);
  return fwrite(array, sizeof(unsigned char), 4, stream);
}

size_t fwriteShortLSB(short int data, FILE *stream) {
  unsigned char array[2];

  array[1] = (unsigned char)((data >> 8) & 0xFF);
  array[0] = (unsigned char)(data & 0xFF);
  return fwrite(array, sizeof(unsigned char), 2, stream);
}

void writeWav(const char *filename, std::vector<float> outputSignal) {
  std::cout << "Writing output signal to: " << filename << std::endl;
  FILE *file = fopen(filename, "wb");

  WavHeader header;
  memcpy(header.chunkID, "RIFF", sizeof(int));
  header.chunkSize =
      sizeof(WavHeader) + sizeof(WavData) + outputSignal.size() * sizeof(short);
  memcpy(header.format, "WAVE", sizeof(int));
  memcpy(header.subchunk1ID, "fmt ", sizeof(int));
  header.subchunk1Size = 16;
  header.audioFormat = 1;
  header.numChannels = 1;
  header.sampleRate = 44100;
  header.bitsPerSample = 16;

  fwrite(&header, sizeof(WavHeader), 1, file);
  fwrite("data", sizeof(int), 1, file);

  printWavHeader(header);

  int subchunk2Size = outputSignal.size() * sizeof(short);
  WavData data;
  memcpy(data.subchunk2ID, "data", sizeof(int));
  memcpy(&data.subchunk2Size, &subchunk2Size, sizeof(int));
  fwrite(&data, sizeof(data), 1, file);

  printWavData(data);

  std::vector<short> audioData(outputSignal.size());
  for (size_t i = 0; i < outputSignal.size(); ++i) {
    audioData[i] = static_cast<short>(outputSignal[i] * 32767.0f);
  }
  fwrite(audioData.data(), sizeof(short), audioData.size(), file);

  fclose(file);
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

  inputSignal = readWav(argv[1]);
  impulseResponse = readWav(argv[2]);

  if (inputSignal.empty() || impulseResponse.empty()) {
    std::cerr << "Error: inputfile or impulse response is empty" << std::endl;
    return 1;
  }

  outputSignal = convolve(inputSignal, impulseResponse);
  writeWav(argv[3], outputSignal);

  return 0;
}

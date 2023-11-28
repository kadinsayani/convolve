#include <iostream>
#include <vector>

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

  void print() const {
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
};

class WavData {
public:
  char subchunk2ID[4];
  int subchunk2Size;

  void print() const {
    std::cout << subchunk2ID << std::endl;
    std::cout << subchunk2Size << std::endl;
  }
};

class WavFile {
public:
  WavHeader header;
  WavData data;
  std::vector<float> audioData;

  void read(const char *filename) {
    std::cout << "Reading WAV file: " << filename << std::endl;
    FILE *file = fopen(filename, "rb");

    fread(&header, sizeof(header), 1, file);

    if (header.subchunk1Size != 16) {
      int rem = header.subchunk1Size - 16;
      char nullBytes[rem];
      fread(nullBytes, rem, 1, file);
    }

    fread(&data, sizeof(data), 1, file);

    int numSamples = data.subchunk2Size / (header.bitsPerSample / 8);
    std::vector<short> audio(numSamples);
    fread(audio.data(), sizeof(short), numSamples, file);
    audioData.resize(numSamples);

    for (size_t i = 0; i < numSamples; i++) {
      audioData[i] = static_cast<float>(audio[i]) / 32768.0f;
    }

    fclose(file);
  }

  void write(const char *filename) {
    std::cout << "Writing output signal to: " << filename << std::endl;
    FILE *file = fopen(filename, "wb");

    memcpy(header.chunkID, "RIFF", sizeof(int));
    header.chunkSize =
        sizeof(WavHeader) + sizeof(WavData) + audioData.size() * sizeof(short);
    memcpy(header.format, "WAVE", sizeof(int));
    memcpy(header.subchunk1ID, "fmt ", sizeof(int));
    header.subchunk1Size = 16;
    header.audioFormat = 1;
    header.numChannels = 1;
    header.sampleRate = 44100;
    header.bitsPerSample = 16;

    fwrite(&header, sizeof(WavHeader), 1, file);
    fwrite("data", sizeof(int), 1, file);
    int subchunk2Size = audioData.size() * sizeof(short);
    memcpy(data.subchunk2ID, "data", sizeof(int));
    memcpy(&data.subchunk2Size, &subchunk2Size, sizeof(int));
    fwrite(&data, sizeof(WavData), 1, file);

    int numSamples = data.subchunk2Size / (header.bitsPerSample / 8);
    std::vector<short> outputAudioData(numSamples);

    float maxVal = *std::max_element(audioData.begin(), audioData.end());
    float minVal = *std::min_element(audioData.begin(), audioData.end());
    float range = maxVal - minVal;
    for (size_t i = 0; i < numSamples; i++) {
      audioData[i] = (audioData[i] - minVal) / range;
      float scaledValue = audioData[i] * 32767.0f;
      scaledValue = std::max(-32768.0f, std::min(scaledValue, 32767.0f));
      outputAudioData[i] = static_cast<short>(scaledValue);
    }

    fwrite(outputAudioData.data(), sizeof(short), numSamples, file);

    fclose(file);
  }
};

class Convolve {
public:
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
};

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: convolve inputfile IRfile outputfile" << std::endl;
    return 1;
  }

  WavFile inputWav, irWav, outputWav;
  Convolve convolve;

  inputWav.read(argv[1]);
  irWav.read(argv[2]);

  if (inputWav.audioData.empty() || irWav.audioData.empty()) {
    std::cerr << "Error: inputfile or impulse response is empty" << std::endl;
    return 1;
  }

  outputWav.audioData = convolve.convolve(inputWav.audioData, irWav.audioData);
  outputWav.write(argv[3]);

  return 0;
}

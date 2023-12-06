#include "WavFile.h"

using namespace Convolve;

void WavFile::read(const char *filename) {
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

  float scalingFactor = 1.0f / 32768.0f;
  for (size_t i = 0; i < numSamples; i++) {
    audioData[i] = static_cast<float>(audio[i]) * scalingFactor;
  }

  fclose(file);
}

void WavFile::write(const char *filename) {
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
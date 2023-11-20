#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MONO 1
#define STEREO 2

struct WavFile {
  char chunk_id[5];
  int chunk_size;
  char format[5];
  char subchunk1_id[5];
  char subchunk1_size;
  int audio_format;
  int n_channels;
  int sample_rate;
  int byte_rate;
  int block_align;
  int bits_per_sample;
  char subchunk2_id[5];
  int subchunk2_size;
};

void convolve(float x[], int N, float h[], int M, float y[], int P);
void print_vector(char *title, float x[], int N);
void read_wav_file(FILE *input_file, struct WavFile *wav, float *input_signal,
                   int *input_size);
void write_wav_file(int channels, int n_samples, double output_rate,
                    FILE *output_file);
size_t fwrite_int_LSB(int data, FILE *stream);
size_t fwrite_short_LSB(short int data, FILE *stream);

int main(int argc, char *argv[]) {
  float input_signal[100], impulse_response[20], output_signal[120];
  int input_size, impulse_size, output_size;

  char *output_file = NULL;

  if (argc != 3) {
    output_file = argv[3];
  } else {
    fprintf(stderr, "Usage: %s inputfile IRfile outputfile\n", argv[0]);
    exit(-1);
  }

  FILE *input_file = fopen(argv[1], "rb");
  if (input_file == NULL) {
    fprintf(stderr, "Error opening inputfile\n");
  }

  struct WavFile wav;
  read_wav_file(input_file, &wav, input_signal, &input_size);

  fclose(input_file);

  print_vector("Original input signal", input_signal, input_size);

  impulse_response[0] = 0.5;
  impulse_size = 1;

  output_size = input_size + impulse_size - 1;

  convolve(input_signal, input_size, impulse_response, impulse_size,
           output_signal, output_size);
  print_vector("Output singal using IR", output_signal, output_size);
}

void convolve(float x[], int N, float h[], int M, float y[], int P) {
  int n, m;

  if (P != (N + M - 1)) {
    printf("Output signal vector is the wrong size\n");
    printf("It is %-d, but should be %-d\n", P, (N + M - 1));
    return;
  }

  for (n = 0; n < P; n++) {
    y[n] = 0.0;
  }

  for (n = 0; n < N; n++) {
    for (m = 0; m < M; m++) {
      y[n + m] += x[n] * h[m];
    }
  }
}

void print_vector(char *title, float x[], int N) {
  int i;

  printf("\n%s\n", title);
  printf("Vector size: %-d\n", N);
  for (i = 0; i < N; i++) {
    printf("%-d\t\t%f\n", i, x[i]);
  }
}

void read_wav_file(FILE *input_file, struct WavFile *wav, float *input_signal,
                   int *input_size) {
  fread(wav->chunk_id, sizeof(char), 4, input_file);
  fread(&wav->chunk_size, sizeof(int), 1, input_file);
  fread(wav->format, sizeof(char), 4, input_file);
  fread(wav->subchunk1_id, sizeof(char), 4, input_file);
  fread(&wav->subchunk1_size, sizeof(int), 1, input_file);
  fread(&wav->audio_format, sizeof(int), 1, input_file);
  fread(&wav->n_channels, sizeof(int), 1, input_file);
  fread(&wav->sample_rate, sizeof(int), 1, input_file);
  fread(&wav->byte_rate, sizeof(int), 1, input_file);
  fread(&wav->block_align, sizeof(int), 1, input_file);
  fread(&wav->bits_per_sample, sizeof(int), 1, input_file);
  fread(wav->subchunk2_id, sizeof(char), 4, input_file);
  fread(&wav->subchunk2_size, sizeof(int), 1, input_file);

  short int sample;
  *input_size = wav->subchunk2_size / (wav->bits_per_sample / 8);

  for (int i = 0; i < *input_size; i++) {
    fread(&sample, sizeof(short int), 1, input_file);
    input_signal[i] = sample / 32767.0;
  }
}
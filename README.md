# convolve

## CPSC 501 A4 Report

## Timing

All programs timed using `time ./convolve guitar.wav big_hall_mono.wav output.wav`

| Version | Time                                             |
| ------- | ------------------------------------------------ |
| v1.0    | 830.50s user 0.22s system 99% cpu 13:53.90 total |
| v2.0    |                                                  |

## v1.0 - Baseline Program

The baseline program implements a convolution algorithm directly in the time domain. 

## v2.0 - Algorithm-Based Optimization

The algorithm-based optimized program re-implements the convolution algorithm using a frequency-domain algorithm known as the overlap-add FFT convolution algorithm.

## Regression Testing

| Unit Test                       | v1.0 | v2.0 |
| ------------------------------- | ---- | ---- |
| Convolve function test          | ✅    |      |
| WavFile read                    | ✅    |      |
| WavFile write                   | ✅    |      |
| Convolver convolve on wav files | ✅    |      |
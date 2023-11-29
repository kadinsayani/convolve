# convolve

## CPSC 501 A4 Report

## v1.0 - Baseline Program

The baseline program implements a convolution algorithm directly in the time domain. 

## v2.0 - Algorithm-Based Optimization

The algorithm-based optimized program re-implements the convolution algorithm using a frequency-domain algorithm known as the overlap-add FFT convolution algorithm.

## Timing

All programs were timed using `time ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./build/src/output.wav`.

| Version | Time                                             |
| ------- | ------------------------------------------------ |
| v1.0    | 830.50s user 0.22s system 99% cpu 13:53.90 total |
| v2.0    |                                                  |

## Profiling

All programs were profiled using `xcrun xctrace record -t "Time Profiler" --launch ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./build/src/output.wav`.

| Version | Profile Location  |
| ------- | ----------------- |
| v1.0    | ./profilev1.0.txt |
| v2.0    |                   |

## Regression Testing

| Unit Test                       | v1.0 | v2.0 |
| ------------------------------- | ---- | ---- |
| Convolve function test          | ✅    |      |
| WavFile read                    | ✅    |      |
| WavFile write                   | ✅    |      |
| Convolver convolve on wav files | ✅    |      |
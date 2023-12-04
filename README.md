# convolve

## CPSC 501 A4 Report

## v1.0 - Baseline Program

The baseline program implements a convolution algorithm directly in the time domain. 

```C++
std::vector<float> Convolver::convolve(const std::vector<float> &x,
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
```

## v2.0 - Algorithm-Based Optimization

The algorithm-based optimized program re-implements the convolution algorithm using a frequency-domain algorithm known as the overlap-add FFT convolution algorithm.

```C++
```

## Timing

All programs were timed using `time ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./build/src/output.wav`.

| Version | Time                                             |
| ------- | ------------------------------------------------ |
| v1.0    | 830.50s user 0.22s system 99% cpu 13:53.90 total |
| v2.0    | 3.56s user 0.02s system 99% cpu 3.592 total      |

## Profiling

All programs were profiled using `xcrun xctrace record -t "Time Profiler" --launch ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./build/src/output.wav`.

| Version | Profile Location  |
| ------- | ----------------- |
| v1.0    | ./profilev1.0.txt |
| v2.0    | ./profilev2.0.txt |

### Flamegraphs

#### v2.0

![flamegraphv2.0](./flamegraphv2.0.svg)

## Regression Testing

| Unit Test                       | v1.0 | v2.0 |
| ------------------------------- | ---- | ---- |
| Convolve function test          | ✅    | ✅    |
| WavFile read                    | ✅    | ✅    |
| WavFile write                   | ✅    | ✅    |
| Convolver convolve on wav files | ✅    | ✅    |
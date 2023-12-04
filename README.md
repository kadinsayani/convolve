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

The algorithm-based optimized program re-implements the convolution algorithm using a frequency-domain FFT convolution algorithm.

```C++
std::vector<float> Convolver::convolve(const std::vector<float> &x,
                                       const std::vector<float> &h) {
  std::cout << "Convolving input signal and impulse response" << std::endl;

  size_t outputSize = x.size() + h.size() - 1;
  size_t nn = pow(2, ceil(log2(x.size() * 2)));
  size_t k, real, imag;

  std::vector<float> X(nn * 2 + 1, 0.0f);
  std::vector<float> H(nn * 2 + 1, 0.0f);
  std::vector<float> Y(nn * 2 + 1, 0.0f);

  for (k = 0; k < x.size(); k++) {
    real = (k * 2) + 1;
    imag = real + 1;
    X[real] = x[k];
    X[imag] = 0.0f;
  }

  for (k = 0; k < h.size(); k++) {
    real = (k * 2) + 1;
    imag = real + 1;
    H[real] = h[k];
    H[imag] = 0.0f;
  }

  fft(X, nn, 1);
  fft(H, nn, 1);

  for (k = 0; k < nn; k++) {
    real = (k * 2) + 1;
    imag = real + 1;
    float Xr = X[real], Xi = X[imag];
    float Hr = H[real], Hi = H[imag];
    Y[real] = (Xr * Hr) - (Xi * Hi);
    Y[imag] = (Xr * Hi) + (Xi * Hr);
  }

  fft(Y, nn, -1);

  for (k = 0; k <= nn; k++) {
    real = (k * 2) + 1;
    Y[real] /= static_cast<float>(nn);
  }

  std::vector<float> result(outputSize);
  for (size_t k = 0; k < outputSize; k++) {
    real = (k * 2) + 1;
    result[k] = Y[real];
  }

  return result;
}
```

## v3.0 - Compiler-Based Optimization

The compiler-based optimization version compiles source code with the gcc/g++ -O3 flag set.

## Timing

All programs were timed using `time ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./tests/output<version>.wav`.

| Version | Time                                             |
| ------- | ------------------------------------------------ |
| v1.0    | 830.50s user 0.22s system 99% cpu 13:53.90 total |
| v2.0    | 3.56s user 0.02s system 99% cpu 3.592 total      |
| v3.0    | 1.43s user 0.03s system 83% cpu 1.735 total      |

## Profiling

All programs were profiled using `xcrun xctrace record -t "Time Profiler" --launch ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./tests/output<version>.wav`. 

| Version | Profile Location  |
| ------- | ----------------- |
| v1.0    | ./profilev1.0.txt |
| v2.0    | ./profilev2.0.txt |

### Flamegraphs

All programs were also profiled by creating and examining flamegraphs by running `flamegraph --output flamegraph<version>.svg --root -- ./build/src/convolve ./build/src/guitar.wav ./build/src/big_hall_mono.wav ./tests/output<version>.wav`

#### v2.0

![flamegraphv2.0](./flamegraphv2.0.svg)

### v3.0

![flamegraphv3.0](./flamegraphv3.0.svg)

### v4.0

### v4.1

### v4.2

### v4.3

### v4.4

## Regression Testing

Regression testing is accomplished by comparing the output.wav files between versions, ensuring convolve() produces the same result. Running `python3 ./tests/audiodiff.py` compares the output wav files by comparing the frames produced by all versions of convolve(). Below is the output produced by `audiodiff.py`.

```zsh
❯ python3 /Users/kadinsayani/dev/convolve/tests/audiodiff.py


tests/outputv1.0.wav
Number of channels 1
Sample width 2
Frame rate 44100
Number of frames 817508530


tests/outputv2.0.wav
Number of channels 1
Sample width 2
Frame rate 44100
Number of frames 817508530


tests/outputv3.0.wav
Number of channels 1
Sample width 2
Frame rate 44100
Number of frames 817508530


Comparing frames...
Output wav files are equal
--------------------------
All tests passed ✅
```

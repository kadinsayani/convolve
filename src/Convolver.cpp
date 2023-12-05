#include "Convolver.h"

using namespace Convolve;

void parallelLoop(std::vector<float> &vec, const std::vector<float> &src,
                  size_t start, size_t end) {
  for (size_t k = start; k < end; k++) {
    size_t real = (k * 2) + 1;
    size_t imag = real + 1;
    vec[real] = src[k];
    vec[imag] = 0.0f;
  }
}

std::vector<float> Convolver::convolve(const std::vector<float> &x,
                                       const std::vector<float> &h) {
  std::cout << "Convolving input signal and impulse response" << std::endl;

  size_t outputSize = x.size() + h.size() - 1;
  size_t nn = x.size() * 2;
  nn--;
  nn |= nn >> 1;
  nn |= nn >> 2;
  nn |= nn >> 4;
  nn |= nn >> 8;
  nn |= nn >> 16;
  nn |= nn >> 32;
  nn++;
  size_t k, real, imag;

  std::vector<float> X(0.0f);
  X.reserve(nn * 2 + 1);
  std::vector<float> H(0.0f);
  H.reserve(nn * 2 + 1);
  std::vector<float> Y(0.0f);
  Y.reserve(nn * 2 + 1);

  const size_t numThreads = std::thread::hardware_concurrency();
  size_t chunkSizeX = x.size() / numThreads;
  size_t chunkSizeH = h.size() / numThreads;

  std::vector<std::thread> threadsX(numThreads);
  for (size_t i = 0; i < numThreads; i++) {
    threadsX[i] = std::thread(parallelLoop, std::ref(X), std::cref(x),
                              i * chunkSizeX, (i + 1) * chunkSizeX);
  }

  for (auto &thread : threadsX) {
    thread.join();
  }

  std::vector<std::thread> threadsH(numThreads);
  for (size_t i = 0; i < numThreads; i++) {
    threadsH[i] = std::thread(parallelLoop, std::ref(H), std::cref(h),
                              i * chunkSizeH, (i + 1) * chunkSizeH);
  }

  for (auto &thread : threadsH) {
    thread.join();
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

  float inv_nn = 1.0f / static_cast<float>(nn);
  for (k = 0; k <= nn; k++) {
    real = (k * 2) + 1;
    Y[real] *= inv_nn;
  }

  std::vector<float> result(outputSize);
  for (size_t k = 0; k < outputSize; k++) {
    real = (k * 2) + 1;
    result[k] = Y[real];
  }

  return result;
}

void Convolver::fft(std::vector<float> &data, int nn, int isign) {
  unsigned long n, mmax, m, j, istep, i;
  double wtemp, wr, wpr, wpi, wi, theta;
  float tempr, tempi;

  n = nn << 1;
  j = 1;

  for (i = 1; i < n; i += 2) {
    if (j > i) {
      std::swap(data[j], data[i]);
      std::swap(data[j + 1], data[i + 1]);
    }
    m = nn;
    while (m >= 2 && j > m) {
      j -= m;
      m >>= 1;
    }
    j += m;
  }

  mmax = 2;
  while (n > mmax) {
    istep = mmax << 1;
    theta = isign * (6.28318530717959 / mmax);
    wtemp = sin(0.5 * theta);
    wpr = -2.0 * wtemp * wtemp;
    wpi = sin(theta);
    wr = 1.0;
    wi = 0.0;
    for (m = 1; m < mmax; m += 2) {
      for (i = m; i <= n; i += istep) {
        j = i + mmax;
        tempr = wr * data[j] - wi * data[j + 1];
        tempi = wr * data[j + 1] + wi * data[j];
        data[j] = data[i] - tempr;
        data[j + 1] = data[i + 1] - tempi;
        data[i] += tempr;
        data[i + 1] += tempi;
      }
      wr = (wtemp = wr) * wpr - wi * wpi + wr;
      wi = wi * wpr + wtemp * wpi + wi;
    }
    mmax = istep;
  }
}

#include "Convolver.h"

using namespace Convolve;

#define SWAP(a, b)                                                             \
  tempr = (a);                                                                 \
  (a) = (b);                                                                   \
  (b) = tempr

std::vector<float> Convolver::convolve(const std::vector<float> &x,
                                       const std::vector<float> &h) {
  std::cout << "Convolving input signal and impulse response" << std::endl;

  int N = x.size();
  int M = h.size();

  int nn = 1;
  // nn must be a power of 2
  do {
    nn *= 2;
  } while (nn <= (N + M - 1));

  std::vector<float> paddedX(nn, 0.0);
  std::vector<float> paddedH(nn, 0.0);
  std::copy(x.begin(), x.end(), paddedX.begin());
  std::copy(h.begin(), h.end(), paddedH.begin());

  // convolve
  fft(paddedX, nn, 1);
  fft(paddedH, nn, 1);

  for (size_t i = 0; i < nn; i++) {
    paddedX[i] *= paddedH[i];
  }

  fft(paddedX, nn, -1);

  std::vector<float> y(nn);
  for (size_t i = 0; i < nn; i++) {
    y[i] = paddedX[i];
  }

  for (size_t k = 0, i = 0; k < N; k++, i += 2) {
    y[i] /= (float)N;
    y[i + 1] /= (float)N;
  }

  return y;
}

void Convolver::createComplexData(const std::vector<float> &input,
                                  float *data) {
  int i, ii;

  for (i = 0, ii = 0; i < input.size(); i++, ii += 2) {
    data[ii] = input[i];
    data[ii + 1] = 0.0;
  }
}

void Convolver::fft(std::vector<float> &data, int nn, int isign) {
  unsigned long n, mmax, m, j, istep, i;
  double wtemp, wr, wpr, wpi, wi, theta;
  float tempr, tempi;

  n = nn << 1;
  j = 1;

  for (i = 1; i < n; i += 2) {
    if (j > i) {
      SWAP(data[j], data[i]);
      SWAP(data[j + 1], data[i + 1]);
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

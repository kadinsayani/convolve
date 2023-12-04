#include "Convolver.h"

using namespace Convolve;

std::vector<float> Convolver::convolve(const std::vector<float> &x,
                                       const std::vector<float> &h) {
  std::cout << "Convolving input signal and impulse response" << std::endl;

  size_t outputSize = x.size() + h.size() - 1;
  size_t nn = pow(2, ceil(log2(x.size() * 2)));
  size_t k, real, imag;

  std::vector<float> X(0.0f);
  X.reserve(nn * 2 + 1);
  std::vector<float> H(0.0f);
  H.reserve(nn * 2 + 1);
  std::vector<float> Y(0.0f);
  Y.reserve(nn * 2 + 1);

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

// fft assumes array starts at index 1
// data is complex so array size must be nn * 2
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

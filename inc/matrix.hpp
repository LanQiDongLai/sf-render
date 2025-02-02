#pragma once
#include <cstring>

#include "vector.hpp"

namespace sf {
template <class T, int N>
class Matrix {
 public:
  Matrix() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i == j) {
          data[i][j] = 1;
        } else {
          data[i][j] = 0;
        }
      }
    }
  }
  Matrix& operator=(const Matrix<T, N>& matrix) {
    std::memmove(data, matrix.data, N * N * sizeof(T));
    return *this;
  }
  Matrix operator*(const Matrix<T, N>& other) {
    Matrix<T, N> res;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        res.data[i][j] = 0;
        for (int k = 0; k < N; k++) {
          res.data[i][j] += data[i][k] * other.data[k][j];
        }
      }
    }
    return res;
  }
  Matrix inverse() {
    Matrix inv;
    T aug[N][2 * N];

    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        aug[i][j] = data[i][j];
      }
      for (int j = N; j < 2 * N; ++j) {
        aug[i][j] = (j - N == i) ? static_cast<T>(1) : static_cast<T>(0);
      }
    }

    for (int i = 0; i < N; ++i) {
      int pivot = i;
      for (int j = i; j < N; ++j) {
        if (std::abs(aug[j][i]) > std::abs(aug[pivot][i])) {
          pivot = j;
        }
      }

      if (aug[pivot][i] == 0) {
        throw std::runtime_error("[MATRIX IS SINGULAR]");
      }

      if (pivot != i) {
        for (int j = 0; j < 2 * N; ++j) {
          std::swap(aug[i][j], aug[pivot][j]);
        }
      }

      T divisor = aug[i][i];
      for (int j = i; j < 2 * N; ++j) {
        aug[i][j] /= divisor;
      }

      for (int k = 0; k < N; ++k) {
        if (k != i) {
          T factor = aug[k][i];
          for (int j = i; j < 2 * N; ++j) {
            aug[k][j] -= factor * aug[i][j];
          }
        }
      }
    }

    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        inv.data[i][j] = aug[i][j + N];
      }
    }

    return inv;
  }

  Vector<T, N>& operator[](size_t idx) {
    return data[idx];
  }

  const Vector<T, N>& operator[](size_t idx) const {
    return data[idx];
  }

  T* get_ptr() { return &data[0][0]; }

 private:
  Vector<T, N> data[N];
};

}  // namespace sf

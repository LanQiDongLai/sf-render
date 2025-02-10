#pragma once

namespace sf {

template <class T, int N>
class Vector {
 public:
  Vector(){
    memset(data, 0, N * sizeof(T));
  }
  Vector(const Vector& vec) {
    std::memmove(data, vec.data, N * sizeof(T));
  }
  T* get_ptr() {
    return &data[0];
  }
  T& operator[](size_t idx) {
    return data[idx];
  }
  const T& operator[](size_t idx) const {
    return data[idx];
  }
  Vector operator*(T n) const {
    Vector res;
    for(int i = 0; i < N; i++) {
      res.data[i] = n * data[i];
    }
    return res;
  }
  Vector operator/(T n) const {
    Vector res;
    for(int i = 0; i < N; i++) {
      res.data[i] = data[i] / n;
    }
    return res;
  }
  Vector operator+(const Vector& vec) const {
    Vector res;
    for(int i = 0; i < N; i++) {
      res.data[i] = vec.data[i] + data[i];
    }
    return res;
  }
  T data[N];
};

}
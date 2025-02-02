#pragma once

namespace sf {

template <class T, int N>
class Vector {
 public:
  T* get_ptr() {
    return &data[0];
  }
  T& operator[](size_t idx) {
    return data[idx];
  }
  const T& operator[](size_t idx) const {
    return data[idx];
  }
 private:
  T data[N];
};

}
#pragma once
#include "matrix.hpp"
#include <cmath>

namespace sf {
namespace Transform {

template <class T>
Matrix<T, 4> translate(const Matrix<T, 4>& mat, const Vector<T, 3>& vec) {
  Matrix<T, 4> translate_mat;
  translate_mat[0][3] = vec[0];
  translate_mat[1][3] = vec[1];
  translate_mat[2][3] = vec[2];
  return mat * translate_mat;
}
template <class T>
Matrix<T, 4> rotate(const Matrix<T, 4>& mat, float angle,
                    const Vector<T, 3>& vec) {
  T vec_length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
  if(vec_length == 0)
    return mat;
  
  Vector<T, 3> tvec = vec;
  tvec = tvec / vec_length;
  float half_angle = angle / 2.f;
  T const w = std::cos(half_angle);
  T const sin_half = std::sin(half_angle);
  T const qx = tvec[0] * sin_half;
  T const qy = tvec[1] * sin_half;
  T const qz = tvec[2] * sin_half;

  Matrix<T, 4> rotate_mat;
  rotate_mat[0][0] = 1 - 2 * qy * qy - 2 * qz * qz;
  rotate_mat[0][1] = 2 * qx * qy + 2 * w * qz;
  rotate_mat[0][2] = 2 * qx * qy - 2 * w * qy;
  rotate_mat[0][3] = 0;

  rotate_mat[1][0] = 2 * qx * qy - 2 * w * qz;
  rotate_mat[1][1] = 1 - 2 * qx * qx - 2 * qz * qz;
  rotate_mat[1][2] = 2 * qy * qz + 2 * w * qx;
  rotate_mat[1][3] = 0;

  rotate_mat[2][0] = 2 * qx * qz + 2 * w * qy;
  rotate_mat[2][1] = 2 * qy * qz - 2 * w * qx;
  rotate_mat[2][2] = 1 - 2 * qx * qx - 2 * qy * qy;
  rotate_mat[2][3] = 0;

  rotate_mat[3][0] = 0;
  rotate_mat[3][1] = 0;
  rotate_mat[3][2] = 0;
  rotate_mat[3][3] = 1;

  return mat * rotate_mat;
}
template <class T>
Matrix<T, 4> scale(const Matrix<float, 4>& mat, const Vector<T, 3>& vec) {
  Matrix<T, 4> scale_mat;
  scale_mat[0][0] = vec[0];
  scale_mat[1][1] = vec[1];
  scale_mat[2][2] = vec[2];
  return mat * scale_mat;
}

}  // namespace Transform
}  // namespace sf
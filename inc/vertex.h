#pragma once
#include "vector.hpp"

namespace sf {

struct Vertex {
  Vector<float, 4> position;
  Vector<float, 3> color;
  Vector<float, 2> texcoord;
  Vector<float, 3> normal;
};

}  // namespace sf

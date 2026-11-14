
#include <random>

#include "types.h"

Color toColor(float r, float g, float b) { return new float[3]{r, g, b}; }

Color getRandomColor() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  return toColor(dist(mt), dist(mt), dist(mt));
}

#include <random>

#include "types.h"

Color getRandomColor() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  return Color{dist(mt), dist(mt), dist(mt)};
}

bool isOverlapping(Bounds a, Bounds b) {
  bool overlapX = a.x < b.x + b.w && a.x + a.w > b.x;
  bool overlapY = a.y < b.y + b.h && a.y + a.h > b.y;
  return overlapX && overlapY;
}

void createTexture(WindowInfo* windows) {}

#include <random>

#include "types.h"

int paletteColorIndex = 0;

Color getRandomColor() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  paletteColorIndex = -1;
  return Color{dist(mt), dist(mt), dist(mt)};
}

Color getNextPaletteColor() {
  const int PALETTE_COLORS = 5;
  const Color PALETTE[PALETTE_COLORS] = {
      {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0},
  };

  paletteColorIndex = (paletteColorIndex + 1) % PALETTE_COLORS;
  return PALETTE[paletteColorIndex];
}

bool isOverlapping(Bounds lhs, Bounds rhs) {
  bool overlapX = lhs.x < rhs.x + rhs.w && lhs.x + lhs.w > rhs.x;
  bool overlapY = lhs.y < rhs.y + rhs.h && lhs.y + lhs.h > rhs.y;
  return overlapX && overlapY;
}

bool hasPixel(Bounds bounds, int x, int y) {
  return isOverlapping(bounds, Bounds{x, y, 1, 1});
}

Color operator+(Color lhs, Color rhs) {
  return {lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b};
}

Color blend(Color acc, int count) {
  return {acc.r / count, acc.g / count, acc.b / count};
}

Color avgForPixel(std::vector<WindowInfo> windows, int x, int y) {
  Color acc = {0, 0, 0};
  int count = 0;

  for (WindowInfo info : windows) {
    if (hasPixel(info.bounds, x, y)) {
      acc = acc + info.color;
      count++;
    }
  }

  return blend(acc, count);
}

typedef std::vector<Color> ColorMatrix;
ColorMatrix getColorMatrix(Bounds bounds, std::vector<WindowInfo> windows) {
  ColorMatrix matrix;

  for (int y = bounds.y; y < bounds.y + bounds.h; y++) {
    for (int x = bounds.x; x < bounds.x + bounds.w; x++) {
      matrix.push_back(avgForPixel(windows, x, y));
    }
  }

  return matrix;
}
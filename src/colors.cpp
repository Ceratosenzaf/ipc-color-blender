
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

Color operator+(Color lhs, Color rhs) {
  return {lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b};
}

void blend(Color* acc, int* count) {
  if (*count <= 1) return;
  acc->r /= *count;
  acc->g /= *count;
  acc->b /= *count;
}

int max(int lhs, int rhs) {
  if (lhs > rhs) return lhs;
  return rhs;
}

int min(int lhs, int rhs) {
  if (lhs < rhs) return lhs;
  return rhs;
}

void restrictToBounds(Bounds* src, Bounds* dst) {
  int maxX = min(src->x + src->w, dst->x + dst->w);
  int maxY = min(src->y + src->h, dst->y + dst->h);
  src->x = max(src->x, dst->x);
  src->y = max(src->y, dst->y);
  src->w = max(maxX - src->x, 0);
  src->h = max(maxY - src->y, 0);
}

typedef std::vector<Color> ColorMatrix;
ColorMatrix getColorMatrix(Bounds* bounds, std::vector<WindowInfo>* windows) {
  ColorMatrix matrix(bounds->w * bounds->h, Color{0, 0, 0});
  std::vector<int> counts(bounds->w * bounds->h, 0);

  for (WindowInfo& info : *windows) {
    restrictToBounds(&info.bounds, bounds);

    if (info.bounds.w <= 0 || info.bounds.h <= 0) continue;

    for (int j = 0; j < info.bounds.h; j++) {
      int rowOffset = (info.bounds.y + j - bounds->y) * bounds->w;
      int cellOffset = rowOffset + info.bounds.x - bounds->x;

      Color* rowMatrix = matrix.data() + cellOffset;
      int* rowCounts = counts.data() + cellOffset;

      for (int i = 0; i < info.bounds.w; i++) {
        rowMatrix[i] = rowMatrix[i] + info.color;
        rowCounts[i]++;
      }
    }
  }

  for (int j = 0; j < bounds->h; j++) {
    int rowOffset = j * bounds->w;

    Color* rowMatrix = matrix.data() + rowOffset;
    int* rowCounts = counts.data() + rowOffset;

    for (int i = 0; i < bounds->w; i++) {
      blend(&rowMatrix[i], &rowCounts[i]);
    }
  }

  return matrix;
}
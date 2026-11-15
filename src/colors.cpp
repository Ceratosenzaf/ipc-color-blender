#include <algorithm>
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

void restrictToBounds(Bounds* src, Bounds* dst) {
  int maxX = std::min(src->x + src->w, dst->x + dst->w);
  int maxY = std::min(src->y + src->h, dst->y + dst->h);
  src->x = std::max(src->x, dst->x);
  src->y = std::max(src->y, dst->y);
  src->w = std::max(maxX - src->x, 0);
  src->h = std::max(maxY - src->y, 0);
}

typedef std::vector<Color> ColorMatrix;
ColorMatrix getColorMatrix(Bounds* bounds, std::vector<WindowInfo>* windows) {
  int totalPixels = bounds->w * bounds->h;

  ColorMatrix matrix(totalPixels, Color{0, 0, 0});
  std::vector<int> counts(totalPixels, 0);

  for (WindowInfo& info : *windows) {
    restrictToBounds(&info.bounds, bounds);

    if (info.bounds.w <= 0 || info.bounds.h <= 0) continue;

    for (int j = 0; j < info.bounds.h; j++) {
      int rowOffset = (info.bounds.y + j - bounds->y) * bounds->w;
      int cellOffset = rowOffset + info.bounds.x - bounds->x;

      Color* rowMatrix = matrix.data() + cellOffset;
      int* rowCounts = counts.data() + cellOffset;

      for (int i = 0; i < info.bounds.w; i++) {
        rowMatrix[i].r += info.color.r;
        rowMatrix[i].g += info.color.g;
        rowMatrix[i].b += info.color.b;
        rowCounts[i]++;
      }
    }
  }

  for (int i = 0; i < totalPixels; i++) {
    int count = counts[i];
    if (count > 1) {
      matrix[i].r /= count;
      matrix[i].g /= count;
      matrix[i].b /= count;
    }
  }

  return matrix;
}
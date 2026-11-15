#pragma once

struct Bounds {
  int x, y;
  int w, h;
};

struct Color {
  float r, g, b;
};

struct WindowInfo {
  Bounds bounds;
  Color color;
  time_t updatedAt;
};

const int SHARED_MEMORY_PAGE_SIZE = 10;
struct SharedMemory {
  WindowInfo windows[SHARED_MEMORY_PAGE_SIZE];  // TODO: handle multiple pages
};
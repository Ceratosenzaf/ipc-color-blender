#pragma once

typedef float* Color;

struct WindowInfo {
  int x, y, w, h;
  float r, g, b;
  time_t updatedAt;
};

const int SHARED_MEMORY_PAGE_SIZE = 10;
struct SharedMemory {
  WindowInfo windows[SHARED_MEMORY_PAGE_SIZE];  // TODO: handle multiple pages
};
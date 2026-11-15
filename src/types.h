#pragma once

struct Bounds {
  int x, y;
  int w, h;
};

struct Color {
  float r, g, b;
};

typedef int64_t Time;

struct WindowInfo {
  Bounds bounds;
  Color color;
  Time updatedAt;
};

const int SHARED_MEMORY_PAGE_SIZE = 10;
struct SharedMemory {
  WindowInfo windows[SHARED_MEMORY_PAGE_SIZE];  // TODO: handle multiple pages
};
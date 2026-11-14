#pragma once

typedef float* Color;

struct WindowInfo {
  int x, y, w, h;
  Color color;
};

struct SharedMemory {
  WindowInfo window;  // TODO: handle multiple windows
};
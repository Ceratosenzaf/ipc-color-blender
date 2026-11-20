
#include <chrono>

#include "types.h"

Time getNow() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

Time getElapsed(Time start) { return getNow() - start; }
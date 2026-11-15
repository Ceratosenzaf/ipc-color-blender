
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <chrono>
#include <vector>

#include "types.h"

int memoryIndex = -1;

Time now() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

bool isActive(WindowInfo* info) {
  const int ACTIVE_THRESHOLD = 100;
  Time elapsed = now() - info->updatedAt;
  return elapsed <= ACTIVE_THRESHOLD;
}

void upsertMemoryIndex(SharedMemory* mem) {
  if (memoryIndex != -1) return;
  for (int i = 0; i < SHARED_MEMORY_PAGE_SIZE; i++) {
    if (!isActive(&(mem->windows)[i])) {
      memoryIndex = i;
      return;
    }
  }
}

void upsertWindowInfo(SharedMemory* mem, WindowInfo* info) {
  upsertMemoryIndex(mem);
  (mem->windows)[memoryIndex] = *info;
  (mem->windows)[memoryIndex].updatedAt = now();
}

std::vector<WindowInfo> handleCommunication(WindowInfo* info) {
  const char* SHARED_MEMORY_NAME = "memory";
  const int MEMORY_SIZE = sizeof(struct SharedMemory);

  int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0600);

  ftruncate(fd, MEMORY_SIZE);

  struct SharedMemory* mem = static_cast<struct SharedMemory*>(
      mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

  upsertWindowInfo(mem, info);

  std::vector<WindowInfo> windows;
  for (WindowInfo& win : mem->windows) {
    if (isActive(&win)) windows.push_back(win);
  }

  return windows;
}
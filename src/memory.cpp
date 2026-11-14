
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "types.h"

const char* SHARED_MEMORY_NAME = "memory";
const int ACTIVE_THRESHOLD = 2;
const int MEMORY_SIZE = sizeof(struct SharedMemory);

int memoryIndex = -1;

time_t now() { return time(0); }

bool isActive(WindowInfo info) {
  time_t elapsed = now() - info.updatedAt;
  return elapsed <= ACTIVE_THRESHOLD;
}

void upsertMemoryIndex(SharedMemory* mem) {
  if (memoryIndex != -1) return;
  for (int i = 0; i < SHARED_MEMORY_PAGE_SIZE; i++) {
    if (!isActive((mem->windows)[i])) {
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

WindowInfo* handleCommunication(WindowInfo* info) {
  int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0600);

  ftruncate(fd, MEMORY_SIZE);

  struct SharedMemory* mem = static_cast<struct SharedMemory*>(
      mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

  upsertWindowInfo(mem, info);

  std::vector<WindowInfo> windows;
  for (WindowInfo win : mem->windows) {
    if (isActive(win)) windows.push_back(win);
  }

  return windows.data();
}
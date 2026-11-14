
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>

#include "types.h"

const int ACTIVE_THRESHOLD = 10;
const int MEMORY_SIZE = sizeof(struct SharedMemory);

int memoryIndex = -1;

time_t getNow() { return time(0); }

bool isActive(WindowInfo info) {
  time_t elapsed = getNow() - info.updatedAt;
  return elapsed <= ACTIVE_THRESHOLD;
}

void upsertMemoryIndex(SharedMemory* data) {
  if (memoryIndex != -1) return;
  for (int i = 0; i < SHARED_MEMORY_PAGE_SIZE; i++) {
    if (!isActive((data->windows)[i])) {
      memoryIndex = i;
      return;
    }
  }
}

void storeInMemory(SharedMemory* data, WindowInfo* info) {
  upsertMemoryIndex(data);
  (data->windows)[memoryIndex] = *info;
  (data->windows)[memoryIndex].updatedAt = getNow();
}

void debugMemory(SharedMemory* data) {
  for (int i = 0; i < SHARED_MEMORY_PAGE_SIZE; i++) {
    WindowInfo info = (data->windows)[i];
    std::cout << i << "," << getpid() << "," << info.x << "," << info.y << ","
              << info.y << "," << info.w << "," << info.h << "," << info.r
              << "," << info.g << "," << info.b << "," << info.updatedAt
              << std::endl;
  }
}

void handleCommunication(WindowInfo* info) {
  int fd = shm_open("test", O_RDWR | O_CREAT, 0600);

  ftruncate(fd, MEMORY_SIZE);

  struct SharedMemory* data = static_cast<struct SharedMemory*>(
      mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

  storeInMemory(data, info);
  debugMemory(data);
}
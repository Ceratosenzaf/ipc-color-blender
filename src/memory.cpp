#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <chrono>
#include <vector>

#include "lock.cpp"
#include "time.cpp"
#include "types.h"

SharedMemory* mem;
int memoryIndex = -1;

const int RUNNING_THRESHOLD = 100;
const int INIT_THRESHOLD = 1000;
bool isActive(WindowInfo* info, int threshold = RUNNING_THRESHOLD) {
  return getElapsed(info->updatedAt) <= threshold;
}

void upsertMemoryIndex() {
  if (memoryIndex != -1) return;

  Lock lock;

  for (int i = 0; i < SHARED_MEMORY_PAGE_SIZE; i++) {
    if (!isActive(&(mem->windows)[i], INIT_THRESHOLD)) {
      memoryIndex = i;
      (mem->windows)[memoryIndex].updatedAt = getNow();
      return;
    }
  }
}

void upsertWindowInfo(WindowInfo* info) {
  upsertMemoryIndex();
  info->updatedAt = getNow();
  (mem->windows)[memoryIndex] = *info;
}

bool setupCommunication() {
  if (mem) return true;

  const char* SHARED_MEMORY_NAME = "/ipc-color-blender.data";
  const int MEMORY_SIZE = sizeof(SharedMemory);

  int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0600);

  if (ftruncate(fd, MEMORY_SIZE) == -1) return false;

  auto* pt = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (!pt) return false;

  close(fd);
  mem = static_cast<SharedMemory*>(pt);

  return true;
}

std::vector<WindowInfo> handleCommunication(WindowInfo* info) {
  std::vector<WindowInfo> windows;

  if (!setupCommunication()) return windows;

  upsertWindowInfo(info);

  for (WindowInfo& win : mem->windows)
    if (isActive(&win)) windows.push_back(win);

  return windows;
}
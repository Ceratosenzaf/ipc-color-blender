
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "types.h"

const int SIZE = sizeof(struct SharedMemory);

void handleCommunication(WindowInfo* winInfo) {
  int fd = shm_open("test", O_RDWR | O_CREAT, 0600);

  ftruncate(fd, SIZE);

  struct SharedMemory* data = static_cast<struct SharedMemory*>(
      mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

  data->window = *winInfo;
}
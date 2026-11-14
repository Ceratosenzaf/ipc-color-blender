#include <unistd.h>

#include "window.cpp"

int main(int argc, char* argv[]) {
  int desiredProcesses = argc >= 2 ? atoi(argv[1]) : 1;

  for (int i = 0; i < desiredProcesses - 1; i++)
    if (fork() == 0) break;

  handleWindow();

  return 0;
}
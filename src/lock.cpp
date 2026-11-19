#include <errno.h>
#include <sys/file.h>
#include <unistd.h>

class Lock {
  const char* LOCK_FILE_PATH = "/tmp/ipc-color-blender.lock";
  int fd = -1;

 public:
  Lock() { lock(); }

  ~Lock() { unlock(); }

  void lock() {
    if (fd != -1) return;

    fd = open(LOCK_FILE_PATH, O_RDWR | O_CREAT, 0600);
    while (flock(fd, LOCK_EX) == -1 && errno == EINTR) {
    }
  }

  void unlock() {
    if (fd == -1) return;

    close(fd);
    fd = -1;
  }
};
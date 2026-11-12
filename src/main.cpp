#include <GLFW/glfw3.h>
#include <unistd.h>

#include <iostream>
// #include <sys/shm.h>
// #include <sys/mman.h>

void spawnProcesses(int count) {
  for (int i = 0; i < count; i++)
    if (fork() == 0) return;
}

int handleError(const char* error) {
  std::cout << "Error: " << error << std::endl;
  glfwTerminate();
  return -1;
}

float* getRandomColors() {
  srand(time(NULL) ^ (getpid() << 16));

  auto getColor = []() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  };

  const float R = getColor();
  const float G = getColor();
  const float B = getColor();

  return new float[]{R, G, B};
}

int main(int argc, char* argv[]) {
  const int desiredProcesses = argc >= 2 ? atoi(argv[1]) : 1;
  spawnProcesses(desiredProcesses - 1);

  if (!glfwInit()) return handleError("Could not initialize GLFW");

  GLFWwindow* win = glfwCreateWindow(400, 400, "test", NULL, NULL);
  if (!win) return handleError("Could  not open window");

  glfwMakeContextCurrent(win);

  float* colors = getRandomColors();
  while (!glfwWindowShouldClose(win)) {
    glClearColor(colors[0], colors[1], colors[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
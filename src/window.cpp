#include <GLFW/glfw3.h>

#include <iostream>

#include "colors.cpp"
#include "memory.cpp"
#include "types.h"

const int WIN_W = 400;
const int WIN_H = 400;
const char* WIN_TITLE = "Color Blender";

const double REFRESH_RATE = 1.0 / 60.0;  // 60FPS
double lastRefresh = 0;

void handleClose(const char* msg, GLFWwindow* win) {
  std::cout << msg << std::endl;
  if (win) glfwDestroyWindow(win);
  glfwTerminate();
}

void changeColorOnClick(GLFWwindow* win, Color* color) {
  glfwSetWindowUserPointer(win, color);
  glfwSetMouseButtonCallback(
      win, [](GLFWwindow* win, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
          auto* color = static_cast<Color*>(glfwGetWindowUserPointer(win));
          *color = getRandomColor();
        }
      });
}

void handleRender(GLFWwindow* win, Color* color) {
  int x, y, w, h;
  glfwGetWindowPos(win, &x, &y);
  glfwGetWindowSize(win, &w, &h);

  WindowInfo info = {x, y, w, h, (*color)[0], (*color)[1], (*color)[2]};
  handleCommunication(&info);

  glClearColor((*color)[0], (*color)[1], (*color)[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(win);
}

void handleTick(GLFWwindow* win, Color* color) {
  double now = glfwGetTime();
  double elapsed = now - lastRefresh;

  if (elapsed >= REFRESH_RATE) {
    handleRender(win, color);
    lastRefresh = now;
  }
}

void handleWindow() {
  if (!glfwInit()) return handleClose("ERROR: Could not initialize GLFW", NULL);

  GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, WIN_TITLE, NULL, NULL);
  if (!win) return handleClose("ERROR: Could  not open window", NULL);

  Color color = getRandomColor();
  changeColorOnClick(win, &color);

  glfwMakeContextCurrent(win);

  while (!glfwWindowShouldClose(win)) {
    handleTick(win, &color);
    glfwPollEvents();
  }

  return handleClose("Window Closed", win);
}
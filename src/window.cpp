#include <GLFW/glfw3.h>

#include <iostream>

#include "colors.cpp"

const int WIN_W = 400;
const int WIN_H = 400;
const char* WIN_TITLE = "Color Blender";

void handleClose(const char* msg, GLFWwindow* win) {
  std::cout << msg << std::endl;
  if (win) glfwDestroyWindow(win);
  glfwTerminate();
}

void changeColorOnClick(GLFWwindow* win, float** color) {
  glfwSetWindowUserPointer(win, color);
  glfwSetMouseButtonCallback(
      win, [](GLFWwindow* win, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
          auto* color = static_cast<float**>(glfwGetWindowUserPointer(win));
          *color = getRandomColor();
        }
      });
}

void handleWindow() {
  if (!glfwInit()) return handleClose("ERROR: Could not initialize GLFW", NULL);

  GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, WIN_TITLE, NULL, NULL);
  if (!win) return handleClose("ERROR: Could  not open window", NULL);

  float* color = getRandomColor();
  changeColorOnClick(win, &color);

  glfwMakeContextCurrent(win);

  while (!glfwWindowShouldClose(win)) {
    glClearColor(color[0], color[1], color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  return handleClose("Window Closed", win);
}
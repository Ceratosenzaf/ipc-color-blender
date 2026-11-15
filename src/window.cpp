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
          Color* color = static_cast<Color*>(glfwGetWindowUserPointer(win));
          *color = getRandomColor();
        }
      });
}

Bounds getBounds(GLFWwindow* win) {
  int x, y, w, h;
  glfwGetWindowPos(win, &x, &y);
  glfwGetWindowSize(win, &w, &h);
  return Bounds{x, y, w, h};
}

void handleRender(GLFWwindow* win, Color* color) {
  WindowInfo info = {getBounds(win), *color};
  ColorMatrix matrix = getColorMatrix(info.bounds, handleCommunication(&info));

  glViewport(0, 0, info.bounds.w, info.bounds.h);
  glClear(GL_COLOR_BUFFER_BIT);

  GLuint textureId;
  glGenTextures(1, &textureId);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.bounds.w, info.bounds.h, 0,
               GL_RGB, GL_FLOAT, matrix.data());

  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_QUADS);

  // Bottom-Left screen -> Bottom-Left matrix
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, -1.0f);

  // Bottom-Right screen -> Bottom-Right matrix
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, -1.0f);

  // Top-Right screen -> Top-Right matrix
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, 1.0f);

  // Top-Left screen -> Top-Left matrix
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, 1.0f);

  glEnd();

  glfwSwapBuffers(win);

  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &textureId);
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
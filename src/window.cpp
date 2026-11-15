#include <GLFW/glfw3.h>

#include "colors.cpp"
#include "memory.cpp"
#include "types.h"

const int WIN_W = 400;
const int WIN_H = 400;
const char* WIN_TITLE = "Color Blender";

const double REFRESH_RATE = 1.0 / 60.0;  // 60FPS
double lastRefresh = 0;

void handleClose(GLFWwindow* win) {
  if (win) glfwDestroyWindow(win);
  glfwTerminate();
}

void changeColorOnClick(GLFWwindow* win, Color* color) {
  glfwSetWindowUserPointer(win, color);
  glfwSetMouseButtonCallback(
      win, [](GLFWwindow* win, int button, int action, int mods) {
        auto updateColor = [&win](Color newColor) {
          Color* color = static_cast<Color*>(glfwGetWindowUserPointer(win));
          *color = newColor;
        };

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
          updateColor(getRandomColor());

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
          updateColor(getNextPaletteColor());
      });
}

Bounds getBounds(GLFWwindow* win) {
  int x, y, w, h;
  glfwGetWindowPos(win, &x, &y);
  glfwGetWindowSize(win, &w, &h);
  return Bounds{x, y, w, h};
}

// at 60FPS we have 16ms to complete rendering
void handleRender(GLFWwindow* win, Color* color) {
  WindowInfo info = {getBounds(win), *color};
  auto windows = handleCommunication(&info);
  ColorMatrix matrix = getColorMatrix(&info.bounds, &windows);

  glViewport(0, 0, info.bounds.w, info.bounds.h);

  GLuint textureId;
  glGenTextures(1, &textureId);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // TODO: recompute only the changed part, not the whole texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.bounds.w, info.bounds.h, 0,
               GL_RGB, GL_FLOAT, matrix.data());

  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_QUADS);

  // bottom-left
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, -1.0f);

  // bottom-right
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, -1.0f);

  // top-right
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, 1.0f);

  // top-left
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
  if (!glfwInit()) return handleClose(NULL);

  GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, WIN_TITLE, NULL, NULL);
  if (!win) return handleClose(NULL);

  Color color = getRandomColor();
  changeColorOnClick(win, &color);

  glfwMakeContextCurrent(win);

  while (!glfwWindowShouldClose(win)) {
    handleTick(win, &color);
    glfwPollEvents();
  }

  return handleClose(win);
}
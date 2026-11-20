#include <GLFW/glfw3.h>

#include <thread>

#include "colors.cpp"
#include "memory.cpp"
#include "types.h"

const int WIN_W = 400;
const int WIN_H = 400;
const char* WIN_TITLE = "Color Blender";
const int WIN_REFRESH_MS = 10;  // 100FPS

GLFWwindow* window;
GLuint textureId;
Color color;

void handleClose() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (textureId) glDeleteTextures(1, &textureId);
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
}

void changeColorOnClick() {
  glfwSetWindowUserPointer(window, &color);
  glfwSetMouseButtonCallback(
      window, [](GLFWwindow* win, int button, int action, int mods) {
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

Bounds getBounds() {
  int x, y, w, h;
  glfwGetWindowPos(window, &x, &y);
  glfwGetWindowSize(window, &w, &h);
  return Bounds{x, y, w, h};
}

void handleRender() {
  WindowInfo info = {getBounds(), color};
  auto windows = handleCommunication(&info);
  ColorMatrix matrix = getColorMatrix(&info.bounds, &windows);

  glViewport(0, 0, info.bounds.w, info.bounds.h);

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

  glfwSwapBuffers(window);
}

void handleTick() {
  Time start = now();
  handleRender();
  Time sleep = WIN_REFRESH_MS - (now() - start);
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
}

void setupWindow() {
  if (!glfwInit()) return handleClose();

  window = glfwCreateWindow(WIN_W, WIN_H, WIN_TITLE, NULL, NULL);
  if (!window) return handleClose();

  color = getRandomColor();
  changeColorOnClick();

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  glGenTextures(1, &textureId);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void handleWindow() {
  setupWindow();

  while (!glfwWindowShouldClose(window)) {
    handleTick();
    glfwPollEvents();
  }

  return handleClose();
}
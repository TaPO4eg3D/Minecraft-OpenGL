#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"

typedef void (*WindowFunc)();

struct Window {
  GLFWwindow *handle;

  int width, height;
  WindowFunc init, destroy, render;

  float deltaTime;

  float mouse_lastx, mouse_lasty;
  float mouse_xoffset, mouse_yoffset;
};

extern struct Window window;

void window_init(WindowFunc init, WindowFunc render, WindowFunc destroy);
void window_loop();

#endif

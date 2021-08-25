#include "window.h"

struct Window window;
static bool _hasMouseCoordinates = false;

static void _process_input(GLFWwindow *handle) {
  window.up = glfwGetKey(handle, GLFW_KEY_SPACE) == GLFW_PRESS;
  window.down = glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
      || glfwGetKey(handle, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

  window.forward = glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS;
  window.backward = glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS;
  window.left = glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS;
  window.right = glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS;
}

static void _framebuffer_size_callback(GLFWwindow *handle, int width, int height) {
  window.width = width;
  window.height = height;

  glViewport(0, 0, width, height);
}

static void _mouse_callback(GLFWwindow *handle, double xpos, double ypos) {
  if (!_hasMouseCoordinates) {
    window.mouse_lastx = xpos;
    window.mouse_lasty = ypos;

    _hasMouseCoordinates = true;
  }

  window.mouse_xoffset = xpos - window.mouse_lastx;
  window.mouse_yoffset = window.mouse_lasty - ypos; // reversed since y-coordinates go from bottom to top

  window.mouse_lastx = xpos;
  window.mouse_lasty = ypos;
}

static void _destroy() {
  window.destroy();

  glfwTerminate();
}

void window_init(WindowFunc init, WindowFunc render, WindowFunc destroy){
  window.width = 800;
  window.height = 800;

  window.init = init;
  window.render = render;
  window.destroy = destroy; 

  window.last_frame = 0.0f;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window.handle = glfwCreateWindow(window.width, window.height, "Minecraft", NULL, NULL);
  if (window.handle == NULL)
  {
      printf("Failed to create a Window!\n");
      glfwTerminate();

      exit(-1);
  }

  glfwMakeContextCurrent(window.handle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      printf("Failed to initialize GLAD! \n");

      exit(-1);
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window.handle, _framebuffer_size_callback);
  glfwSetCursorPosCallback(window.handle, _mouse_callback);
}

void window_loop() {
  window.init();

  // OpenGL Depth test
  glEnable(GL_DEPTH_TEST);
  // Capture Mouse
  glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  while(!glfwWindowShouldClose(window.handle)) {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      float current_frame = glfwGetTime();
      window.delta_time = current_frame - window.last_frame;
      window.last_frame = current_frame;

      _process_input(window.handle);
      window.render();

      glfwSwapBuffers(window.handle);
      glfwPollEvents();    
  }

  _destroy();
}

#include <stdio.h>
#include <stdbool.h>

#include "glfx.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int compile_shader(const char **shader_source, unsigned int shader_type) {
  unsigned int shader;
  shader = glCreateShader(shader_type);

  glShaderSource(shader, 1, shader_source, NULL);
  glCompileShader(shader);

  int  success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);

    printf("Shader compile error!\n");
    printf("%s\n\n", infoLog);
  }

  return shader;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
      printf("Failed to create a Window!\n");
      glfwTerminate();
      return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      printf("Failed to initialize GLAD! \n");
      return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  

  // === WINDOW SET

  const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

  unsigned int vertex_shader = compile_shader(&vertex_shader_source, GL_VERTEX_SHADER);

  const char *fragment_shader_source = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\n";

  unsigned int fragment_shader = compile_shader(&fragment_shader_source, GL_FRAGMENT_SHADER);

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertex_shader);
  glAttachShader(shaderProgram, fragment_shader);

  glLinkProgram(shaderProgram);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // ================================
  // Drawing a triangle
  // ================================

  /* unsigned int VAO, VBO; */
  /* glGenVertexArrays(1, &VAO); */
  /* glGenBuffers(1, &VBO); */

  /* glBindVertexArray(VAO); */

  /* float vertices[] = { */
  /*   -0.5f, -0.5f, 0.0f, */
  /*    0.5f, -0.5f, 0.0f, */
  /*    0.0f,  0.5f, 0.0f */
  /* }; */

  /* glBindBuffer(GL_ARRAY_BUFFER, VBO); */
  /* glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); */

  /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); */
  /* glEnableVertexAttribArray(0); */

  /* glBindBuffer(GL_ARRAY_BUFFER, 0); */
  /* glBindVertexArray(0); */

  // ================================
  // Drawing a rectangle
  // ================================

  float vertices[] = {
    0.0f, 0.0f, 0.0f,
    -0.3f, 0.15f, 0.0f,
    0.3f, 0.15f, 0.0f,
    -0.2f, -0.2f, 0.0f,
    0.0f, 0.3f, 0.0f,
    0.2f, -0.2f, 0.0f,
    0.1f, 0.1f, 0.0f,
    -0.1f, 0.1f, 0.0f
  };

  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,
    3, 4, 6,
    7, 4, 5
  };
  
  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Set display mode to wireframe
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

  while(!glfwWindowShouldClose(window)) {
      process_input(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();    
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();

  return 0;
}

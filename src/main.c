#include <stdio.h>
#include <stdbool.h>

#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> 

#include <cglm/cglm.h>

#include "glfx.h"
#include "shaders.h"

int viewPortWidth = 800;
int viewPortHeight = 600;

float hasMousePosition = false;
float lastX = 400;
float lastY = 300;

float yaw = -90.0f;
float pitch = 0.0f;
float cameraSensetivity = 0.08f;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    viewPortWidth = width;
    viewPortHeight = height;

    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 0.05f; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 tmp;

        glm_vec3_scale(cameraFront, cameraSpeed, tmp);
        glm_vec3_add(cameraPos, tmp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 tmp;

        glm_vec3_scale(cameraFront, cameraSpeed, tmp);
        glm_vec3_sub(cameraPos, tmp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 tmp;

        glm_vec3_crossn(cameraFront, cameraUp, tmp);
        glm_vec3_scale(tmp, cameraSpeed, tmp);
        glm_vec3_sub(cameraPos, tmp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 tmp;

        glm_vec3_crossn(cameraFront, cameraUp, tmp);
        glm_vec3_scale(tmp, cameraSpeed, tmp);
        glm_vec3_add(cameraPos, tmp, cameraPos);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (!hasMousePosition) {
    lastX = xpos;
    lastY = ypos;

    hasMousePosition = true;
  }

  float xoffset = xpos - lastX;
  float yoffset = ypos - lastY;

  lastX = xpos;
  lastY = ypos;
  
  xoffset *= cameraSensetivity;
  yoffset *= cameraSensetivity;

  yaw += xoffset;
  pitch -= yoffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }

  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  vec3 camera_direction = {
    cos(glm_rad(yaw)) * cos(glm_rad(pitch)),
    sin(glm_rad(pitch)),
    sin(glm_rad(yaw)) * cos(glm_rad(pitch))
  };

  glm_vec3_normalize_to(camera_direction, cameraFront);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(viewPortWidth, viewPortHeight, "LearnOpenGL", NULL, NULL);
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
  
  // === WINDOW IS SET

  // ================================
  // Drawing a triangle
  // ================================

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Map position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Map texture
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  int width, height, nrChannels;
  unsigned char *data;

  // Loading texture_0
  data = stbi_load("res/img/container.jpg", &width, &height, &nrChannels, 0);

  GLuint texture0;
  glGenTextures(1, &texture0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture0);

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  stbi_image_free(data);

  // Loading texture_1
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("res/img/awesomeface.png", &width, &height, &nrChannels, 0);

  GLuint texture1;
  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  stbi_image_free(data);

  struct Shader shader = shader_create(
      "res/shaders/base.vert",
      "res/shaders/base.frag",
      2
  );

  shader_use(shader);
  shader_set_int(shader, "texture0", 0);
  shader_set_int(shader, "texture1", 1);

  glEnable(GL_DEPTH_TEST);
  vec3 cubePositions[] = {
      { 0.1f,  0.1f,  0.1f}, 
      { 2.0f,  5.0f, -15.0f}, 
      {-1.5f, -2.2f, -2.5f},  
      {-3.8f, -2.0f, -12.3f},  
      { 2.4f, -0.4f, -3.5f},  
      {-1.7f,  3.0f, -7.5f},  
      { 1.3f, -2.0f, -2.5f},  
      { 1.5f,  2.0f, -2.5f}, 
      { 1.5f,  0.2f, -1.5f}, 
      {-1.3f,  1.0f, -1.5f}  
  };

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  while(!glfwWindowShouldClose(window)) {
      process_input(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader_use(shader);

      // View matrix
      vec3 eye;
      glm_vec3_add(cameraPos, cameraFront, eye);

      mat4 view;
      glm_lookat(cameraPos, eye, cameraUp, view);
      
      // Projection matrix
      mat4 projection;
      glm_perspective(glm_rad(45.0f), viewPortWidth / viewPortHeight, 0.1f, 100.f, projection);

      shader_set_mat4(shader, "view", view);
      shader_set_mat4(shader, "projection", projection);

      glBindVertexArray(VAO);

      for (size_t i = 0; i < 10; i++) {
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model, cubePositions[i]);

        float angle = 80.0f;
        glm_rotate(model, glm_rad(angle) * glfwGetTime(), (vec3){+cubePositions[i][0], +cubePositions[i][1], +cubePositions[i][2]});
        shader_set_mat4(shader, "model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glfwSwapBuffers(window);
      glfwPollEvents();    
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  shader_destroy(shader);

  glfwTerminate();

  return 0;
}

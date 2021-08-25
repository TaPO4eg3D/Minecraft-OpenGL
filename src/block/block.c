#include "block.h"

struct Block block_create(vec3 position, float xslot, float yslot) {
  struct Block self;

  float xoffset = xslot * SPRITE_SIZE;
  float yoffset = TEXTURE_SIZE - (yslot * SPRITE_SIZE);

  float ax = xoffset / TEXTURE_SIZE;
  float ay = (yoffset - SPRITE_SIZE) / TEXTURE_SIZE;

  float bx = (xoffset + SPRITE_SIZE) / TEXTURE_SIZE;
  float by = (yoffset - SPRITE_SIZE) / TEXTURE_SIZE;

  float cx = (xoffset + SPRITE_SIZE) / TEXTURE_SIZE;
  float cy = yoffset / TEXTURE_SIZE;

  float vertices[] = {
        -0.5f, -0.5f, -0.5f, ax, ay,
         0.5f, -0.5f, -0.5f, bx, by,
         0.5f,  0.5f, -0.5f, cx, cy,
         0.5f,  0.5f, -0.5f, cx, cy,
        -0.5f,  0.5f, -0.5f, ax, cy,
        -0.5f, -0.5f, -0.5f, ax, ay,

        -0.5f, -0.5f,  0.5f, ax, ay,
         0.5f, -0.5f,  0.5f, bx, by,
         0.5f,  0.5f,  0.5f, cx, cy,
         0.5f,  0.5f,  0.5f, cx, cy,
        -0.5f,  0.5f,  0.5f, ax, cy,
        -0.5f, -0.5f,  0.5f, ax, ay,

        -0.5f,  0.5f,  0.5f, ax, ay,
        -0.5f,  0.5f, -0.5f, bx, by,
        -0.5f, -0.5f, -0.5f, cx, cy,
        -0.5f, -0.5f, -0.5f, cx, cy,
        -0.5f, -0.5f,  0.5f, ax, cy,
        -0.5f,  0.5f,  0.5f, ax, ay,

         0.5f,  0.5f,  0.5f, ax, ay,
         0.5f,  0.5f, -0.5f, bx, by,
         0.5f, -0.5f, -0.5f, cx, cy,
         0.5f, -0.5f, -0.5f, cx, cy,
         0.5f, -0.5f,  0.5f, ax, cy,
         0.5f,  0.5f,  0.5f, ax, ay,

        -0.5f, -0.5f, -0.5f, ax, ay,
         0.5f, -0.5f, -0.5f, bx, by,
         0.5f, -0.5f,  0.5f, cx, cy,
         0.5f, -0.5f,  0.5f, cx, cy,
        -0.5f, -0.5f,  0.5f, ax, cy,
        -0.5f, -0.5f, -0.5f, ax, ay,

        -0.5f,  0.5f, -0.5f, ax, ay,
         0.5f,  0.5f, -0.5f, bx, by,
         0.5f,  0.5f,  0.5f, cx, cy,
         0.5f,  0.5f,  0.5f, cx, cy,
        -0.5f,  0.5f,  0.5f, ax, cy,
        -0.5f,  0.5f, -0.5f, ax, ay
  };
  glm_vec3_copy(position, self.position);

  // Abstarct VAO and VBO creation
  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  self.vao = vao;
  self.vbo = vbo;

  return self;
}

void block_render(struct Block self, struct Shader shader) {
  // TODO: Probably we should set shader matrix in another place...
  glBindVertexArray(self.vao);

  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});

  shader_set_mat4(shader, "model", model);

  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void block_destroy(struct Block self) {
  free(self.vertices);
}

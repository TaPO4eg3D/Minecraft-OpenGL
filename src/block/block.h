#ifndef BLOCK_H
#define BLOCK_H

#include "../gfx/gfx.h"
#include "../gfx/shaders.h"

struct Block {
  float *vertices;
  size_t vertices_size;

  vec3 position;

  // TODO: Make an abstraction for it
  GLuint vao, vbo;
};

struct Block block_create(vec3 position);
void block_render(struct Block self, struct Shader shader);
void block_destroy(struct Block self);

#endif

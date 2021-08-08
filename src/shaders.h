#ifndef SHADERS_H
#define SHADERS_H

#include "glfx.h"

struct Shader {
  /**
   * Most likely I won't be using any other types of shaders so
   * I decided to combine both Vertex and Fragment shaders in a single Struct
   */
  GLuint shader_program_id;
  GLuint vector_shader_id;
  GLuint fragment_shader_id;
};

/**
 * Compiles both Vertex and Fragment shaders and links them to a shader program
 */
struct Shader shader_create(char *vs_path, char *fs_path, size_t n);

// Binds the shader program
void shader_use(struct Shader self);

// Cleans up the memory
void shader_destroy(struct Shader self);

// Bunch of functions to set shader's uniform based on type value
void shader_set_int(struct Shader self, char *name, int value);
void sahder_set_float(struct Shader self, char *name, float value);

#endif

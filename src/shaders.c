#include <stdio.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "shaders.h"

static void _log_and_fail(
  GLint handle, const char *adverb, const char *path,
  void (*getlog)(GLuint, GLsizei, GLsizei*, GLchar*),
  void (*getiv)(GLuint, GLenum, GLint*)) {

  GLint log_len;
  getiv(handle, GL_INFO_LOG_LENGTH, &log_len);

  char *log_text = malloc(log_len);
  getlog(handle, log_len, NULL, log_text);
  
  fprintf(stderr, "Error %s shader at '%s': \n %s", adverb, path, log_text);

  free(log_text);
  exit(1);
}

static GLuint _compile(char *path, GLenum type) {
  FILE *fp;
  char *source;

  long len;

  fp = fopen(path, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Error loading shader at: \"%s\" \n", path);

    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);

  if (len == 0) {
    fprintf(stderr, "The shader file \"%s\" is empty!", path);
  }

  source = malloc(len);
  assert(source != NULL);

  fread(source, 1, len, fp);
  assert(strlen(source) > 0);

  fclose(fp);

  GLuint handle = glCreateShader(type);
  glShaderSource(handle, 1, (const GLchar *const *) &source, (const GLint *) &len);
  glCompileShader(handle);

  free(source);

  GLint success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

  if (!success)
    _log_and_fail(handle, "compiling", path, glGetShaderInfoLog, glGetShaderiv);

  return handle;
}

struct Shader shader_create(char *vs_path, char *fs_path, size_t n) {
  struct Shader self;

  self.vertex_shader_id = _compile(vs_path, GL_VERTEX_SHADER);
  self.fragment_shader_id = _compile(fs_path, GL_FRAGMENT_SHADER);

  self.shader_program_id = glCreateProgram();

  // Link shaders to a shader program
  glAttachShader(self.shader_program_id, self.vertex_shader_id);
  glAttachShader(self.shader_program_id, self.fragment_shader_id);

  // Bind vertex attributes
  /* for (size_t i = 0; i < n; i++) { */
  /*   glBindAttribLocation(self.shader_program_id, attributes) */
  /* } */

  glLinkProgram(self.shader_program_id);

  // Check link status
  GLint success;
  glGetProgramiv(self.shader_program_id, GL_LINK_STATUS, &success);

  if (!success) {
    char buff[512];
    snprintf(buff, 512, "[%s, %s]", vs_path, fs_path);

    _log_and_fail(self.shader_program_id, "linking", buff, glGetProgramInfoLog, glGetProgramiv);
  }

  return self;
}

void shader_use(struct Shader self) {
  glUseProgram(self.shader_program_id);
}

void shader_destroy(struct Shader self) {
  glDeleteProgram(self.shader_program_id);

  // TODO: Probably it needs to be moved into shader_create function.
  // Because it seems there is no need to store them after shaders have been
  // linked to the shader program
  glDeleteShader(self.vertex_shader_id);
  glDeleteShader(self.fragment_shader_id);
}

void shader_set_int(struct Shader self, char *name, int value) {
  glUniform1i(glGetUniformLocation(self.shader_program_id, name), value);
}

void sahder_set_float(struct Shader self, char *name, float value) {
  glUniform1f(glGetUniformLocation(self.shader_program_id, name), value);
}

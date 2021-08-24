#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"

struct Texture {
  GLuint handle;
  int width, height, nrChannels;
};

struct Texture load_from_path(char *img_path);

// First we set an Active texture then we bind. Order matters

void bind_texture(struct Texture *self);
void set_active_texture(struct Texture *self, int n);

#endif

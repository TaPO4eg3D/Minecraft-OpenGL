#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture load_from_path(char *img_path) {
  struct Texture self;
  unsigned char *data;

  stbi_set_flip_vertically_on_load(true);

  data = stbi_load(img_path, &self.width, &self.height, &self.nrChannels, 0);
  assert(data != NULL);

  glGenTextures(1, &self.handle);
  glBindTexture(GL_TEXTURE_2D, self.handle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self.width, self.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return self;
}

void bind_texture(struct Texture *self) {
  glBindTexture(GL_TEXTURE_2D, self->handle);
}

void set_active_texture(int n) {
  GLenum texture_slots[] = {
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
  };

  glActiveTexture(texture_slots[n]);
}

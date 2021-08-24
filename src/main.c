#include "gfx/window.h"
#include "gfx/camera.h"
#include "gfx/shaders.h"

#include "block/block.h"


struct State {
  struct Camera camera;
  struct Shader shader;
};

struct Block block;
struct State game_state;


void init() {
  game_state.shader = shader_create(
      "res/shaders/base.vert",
      "res/shaders/base.frag",
      1
  );
  shader_use(game_state.shader);

  /* game_state.camera = camera_default_init(); */
  block = block_create((vec3){0.0f, 0.0f, 0.0f});
}

void render() {
  shader_use(game_state.shader);

  /* camera_update(game_state.camera); */
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

  shader_set_mat4(game_state.shader, "view", view);

  mat4 projection;
  glm_perspective(
      glm_rad(45.0f),
      window.width / window.height,
      0.1f, 100.0f,
      projection
  );

  shader_set_mat4(game_state.shader, "projection", projection);

  block_render(block, game_state.shader);
}

void destroy() {
}

int main() {
  window_init(
    init,
    render,
    destroy
  );
  
  window_loop();

  return 0;
}

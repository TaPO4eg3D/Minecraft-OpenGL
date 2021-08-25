#include "camera.h"
#include "window.h"

struct Camera camera_init(float fov, float near_plane, float far_plane, float sensitivity, float movement_speed, vec3 position, vec3 front) {
  struct Camera self;

  self.fov = fov;
  self.near_plane = near_plane;
  self.far_plane = far_plane;

  self.sensitivity = sensitivity;
  self.movement_speed = movement_speed;

  self.yaw = -90.0f;
  self.pitch = 0.0f;

  glm_vec3_copy(front, self.front);
  glm_vec3_copy(position, self.position);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, self.world_up);

  return self;
}

struct Camera camera_default_init() {
  return camera_init(
      DEFAULT_CAMERA_FOV,
      DEFAULT_CAMERA_NEAR_PLANE,
      DEFAULT_CAMERA_FAR_PLANE,
      DEFAULT_CAMERA_SENSITIVITY,
      DEFAULT_CAMERA_SPEED,
      DEFAULT_CAMERA_POSITION,
      DEFAULT_CAMERA_FRONT
  );
}

void camera_update(struct Camera *self) {
  self->yaw += (window.mouse_xoffset * self->sensitivity * window.delta_time);
  self->pitch += (window.mouse_yoffset * self->sensitivity * window.delta_time);

  // TODO: Fix this hack
  window.mouse_xoffset = 0.0f;
  window.mouse_yoffset = 0.0f;

  self->pitch = glm_clamp(self->pitch, -89.0f, 89.0f);

  vec3 front = {
    cos(glm_rad(self->yaw)) * cos(glm_rad(self->pitch)),
    sin(glm_rad(self->pitch)),
    sin(glm_rad(self->yaw)) * cos(glm_rad(self->pitch))
  };

  glm_vec3_normalize_to(front, self->front);
  vec3 scaled_axis_vec;

  if (window.up) {
    glm_vec3_scale_as(self->up, self->movement_speed * window.delta_time, scaled_axis_vec);
    glm_vec3_add(self->position, scaled_axis_vec, self->position);
  }

  if (window.down) {
    glm_vec3_scale_as(self->up, self->movement_speed * window.delta_time, scaled_axis_vec);
    glm_vec3_sub(self->position, scaled_axis_vec, self->position);
  }

  if (window.forward) {
    glm_vec3_scale_as(front, self->movement_speed * window.delta_time, scaled_axis_vec);
    glm_vec3_add(self->position, scaled_axis_vec, self->position);
  }

  if (window.backward) {
    glm_vec3_scale_as(front, self->movement_speed * window.delta_time, scaled_axis_vec);
    glm_vec3_sub(self->position, scaled_axis_vec, self->position);
  }

  if (window.left) {
    glm_vec3_scale_as(self->right, self->movement_speed * window.delta_time, scaled_axis_vec);
    glm_vec3_sub(self->position, scaled_axis_vec, self->position);
  }

  if (window.right) {
    glm_vec3_scale_as(self->right, self->movement_speed * window.delta_time, scaled_axis_vec);
    glm_vec3_add(self->position, scaled_axis_vec, self->position);
  }

  glm_vec3_crossn(self->front, self->world_up, self->right);
  glm_vec3_crossn(self->right, self->front, self->up);

  vec3 position_add_front;
  glm_vec3_add(self->position, self->front, position_add_front);
  glm_lookat(self->position, position_add_front, self->up, self->view);
}

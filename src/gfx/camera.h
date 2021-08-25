#ifndef CAMERA_H
#define CAMERA_H

#include "gfx.h"


#define DEFAULT_CAMERA_FOV 45.0f
#define DEFAULT_CAMERA_NEAR_PLANE 0.1f
#define DEFAULT_CAMERA_FAR_PLANE 100.0f

#define DEFAULT_CAMERA_SPEED 15.0f
#define DEFAULT_CAMERA_SENSITIVITY 30.0f
#define DEFAULT_CAMERA_POSITION ((vec3){ 0.0f, 0.0f, 3.0f })
#define DEFAULT_CAMERA_FRONT ((vec3){ 0.0f, 0.0f, -1.0f })

struct Camera {
  float fov;
  float sensitivity;
  float movement_speed;

  float yaw, pitch;
  float near_plane, far_plane;

  vec3 position, up, right, front;
  vec3 world_up;

  mat4 view;
};

struct Camera camera_init(float fov, float near_plane, float far_plane, float sensitivity, float movement_speed, vec3 position, vec3 front);
// Just a wrapper around "camera_init" with default values
struct Camera camera_default_init();

void camera_update(struct Camera *self);

#endif

#include "camera.h"

Camera::Camera(Point position, tilt_t tilt, fov_t fov)
{
    this->position = position;
    this->tilt = tilt;
    this->fov= fov;
}

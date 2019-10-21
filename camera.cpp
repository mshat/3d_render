#include "camera.h"

Camera::Camera(Point position, tilt_t tilt, vof_t vof)
{
    this->position = position;
    this->tilt = tilt;
    this->vof= vof;
}

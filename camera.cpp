#include "camera.h"

Camera::Camera(Point position, tilt_t tilt, fov_t fov)
{
    this->position = position;
    this->tilt = tilt;
    this->fov= fov;
}

void Camera::move(double step_x, double step_y, double step_z)
{
    position.set_x(position.get_x() + step_x);
    position.set_y(position.get_y() + step_y);
    position.set_z(position.get_z() + step_z);

}

void Camera::rotate(double step_p, double step_y, double step_r)
{
    tilt.pitch += step_p;
    tilt.roll += step_r;
    tilt.yaw += step_y;
}

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "types.h"
#include "geometry.h"

class Camera
{
public:
    Camera() {}
    Camera(Point position, tilt_t tilt, fov_t fov);

    const fov_t get_fov() const { return this->fov; }

private:
    Point position;
    tilt_t tilt;
    fov_t fov;
};

#endif // CAMERA_H

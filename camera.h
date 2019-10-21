#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "types.h"
#include "geometry.h"

class Camera
{
public:
    Camera() {}
    Camera(Point position, tilt_t tilt, vof_t vof);

    const vof_t get_vof() const { return this->vof; }

private:
    Point position;
    tilt_t tilt;
    vof_t vof;
};

#endif // CAMERA_H

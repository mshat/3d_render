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
    Point &get_position() {return  this->position; }
    const tilt_t get_tilt() const {return tilt; }
    void set_tilt(tilt_t tilt) {this->tilt = tilt; }
    void set_position(Point position) {this->position = position; }
    void move(double step_x=0, double step_y=0, double step_z=0);
    void rotate(double step_x=0, double step_y=0, double step_z=0);

private:
    Point position;
    tilt_t tilt;
    fov_t fov;
};

#endif // CAMERA_H

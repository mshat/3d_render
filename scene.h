#ifndef SCENE_H
#define SCENE_H

#include "types.h"
#include "geometry.h"
#include "camera.h"

class Scene
{
public:
    Scene();

    const Camera get_camera() const { return this->camera; }
    const Point get_O() const {return this->O; }
    //void Add_shape(Shape shape);


    //todo убрать нахер
    Sphere sphere;
private:
    //массив объектов

    Point O;
    Camera camera;

};

#endif // SCENE_H

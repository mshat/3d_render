#ifndef SCENE_H
#define SCENE_H

#include "types.h"
#include "geometry.h"
#include "camera.h"
#include "light.h"

class Scene
{
public:
    Scene();

    const Camera get_camera() const { return this->camera; }
    const Point get_O() const {return this->O; }
    Sphere* get_shapes() {return this->shapes; } //todo временно возвращет сферы
    int get_shapes_number() {return this->shapes_number; }
    Light* get_lights() {return  this->lights; }
    int get_lights_number() {return this->lights_number; }
    void Add_shape(Sphere *shape);

    Color base_color = Color(255, 255, 255);

private:

    Sphere shapes[100];
    int shapes_number = 0;
    Light lights[100];
    int lights_number = 0;
    Point O;
    Camera camera;


};

#endif // SCENE_H

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
    std::vector<Shape*> &get_shapes() {return this->shapes; }
    int get_shapes_number() {return this->shapes_number; }
    Light** get_lights() {return  this->lights; }
    int get_lights_number() {return this->lights_number; }
    Camera *get_camera() {return &camera;}
    void set_camera(Camera camera) {this->camera = camera; }

    //Color base_color = Color(0, 0, 50);
    Color base_color = Color(0, 0, 0);
    //Color base_color = Color(255, 255, 255);
    ~Scene();

private:
    std::vector<Shape*> shapes;
    int shapes_number = 0;
    Light *lights[100];
    int lights_number = 0;
    Point O;
    Camera camera;


};

#endif // SCENE_H

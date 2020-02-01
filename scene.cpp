#include "scene.h"

Scene::Scene()
{
    O = Point(0, 0, 0);
    camera = Camera(O, tilt_t(0, 0, 0), fov_t(1, 1, 1));
    shapes[0] = new Sphere(Point(0, -1, 3), Color(255, 0, 0), 500, 0.2, 1);
    //shapes[0] = new Sphere(Point(0, 0, 1), Color(255, 0, 0), 500, 0.2, 0.5);
    shapes[1] = new Sphere(Point(-2, 0, 4), Color(0, 255, 0), 500, 0.4, 1);
    shapes[2] = new Sphere(Point(2, 0, 4), Color(0, 0, 255), 10, 0.3, 1);
    shapes[3] = new Sphere(Point(0, -5001, 0), Color(255, 255, 0), 1000, 0.5, 5000);
    shapes_number = 4;
    lights[0] = new Ambient_light(0.2);
    lights[1] = new Point_light(0.6, Point(2, 1, 0));
    lights[2] = new Directional_light(0.2, Vector(1, 4, 4));
    //lights[2] = new Directional_light(0.2, Vector(0, 1, 0));
    lights_number = 3;

}

void Scene::Add_shape(Sphere *shape)
{
    if (shape == nullptr)
    {
        this->shapes[this->shapes_number + 1] = shape;
        this->shapes_number++;
    }
}

Scene::~Scene()
{
    for (int i = 0; i < shapes_number; i++)
    {
        delete shapes[i];
    }

    for (int i = 0; i < lights_number; i++)
    {
        delete lights[i];
    }
}

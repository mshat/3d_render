#include "scene.h"

Scene::Scene()
{
    O = Point(0, 0, 0);
    camera = Camera(Point(0, 0, 0), tilt_t(0, 0, 0), vof_t(1, 1, 1));
    sphere = Sphere(Point(0, 0, 5), Color(255, 0, 0), 1);
}

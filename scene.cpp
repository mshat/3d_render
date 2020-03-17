#include "scene.h"
#include "math.h"

Scene::Scene()
{
    O = Point(0, -2, -1.5);
    //O = Point(-0.15, -1, -3);
    //camera = Camera(O, tilt_t(0.3, 0.2, 0), fov_t(1, 1, 1));
    camera = Camera(O, tilt_t(0, 0, 0), fov_t(1, 1, 1));

    shapes.push_back(new Sphere(Point(0.21, -2.3, 3), Color(255, 0, 0), 500, 0.9, 0.75));
    shapes.push_back(new Sphere(Point(-1.1, -2.5, 1.8), Color(0, 255, 0), 500, 0.5, 0.5));
    shapes.push_back(new Sphere(Point(1.1, -2.5, 1.8), Color(0, 0, 255), 500, 0.01, 0.5));

    Vector a, b, c;
    int specular = 500;
    double reflective = 0.5;
    Color color(255, 255, 255);
    Color color1(0, 0, 0);
    double step_x = 2.5;
    double step_y = 2.5;
    step_x = 0.9;
    step_y = 0.9;
    //Point left_bottom(-8.5, 0, 3.0);
    Point left_bottom(-2.25, 0, 3.0);
    Point start;
    //int n = 21;
    int n = 5;

    tilt_t tilt(1.5708, 0, 0);

    std::vector<Triangle> triangles;

    Color col;
    int col_n = 0;
    for (int i = 0; i < n; i++)
    {
        start = left_bottom + Point(step_x * i, 0, 0);
        for (int j = 0; j < n; j++)
        {
            col_n += 1;
            if(col_n % 2 == 0)
            {
                col = color;
            }
            else
            {
                col = color1;
            }

            start.set_y(step_y * j);
            a = start;
            b = start + Point(0, step_y, 0);
            c = start + Point(step_x, 0, 0);

            a.rotate_vector(tilt);
            b.rotate_vector(tilt);
            c.rotate_vector(tilt);

            //triangles.push_back(Triangle(a, b, c, col, specular, reflective));
            shapes.push_back(new Triangle(a, b, c, col, specular, reflective));

            start.set_y(step_y * j);
            c = start + Point(step_x, step_y, 0);
            b = start + Point(0, step_y, 0);
            a = start + Point(step_x, 0, 0);

            a.rotate_vector(tilt);
            b.rotate_vector(tilt);
            c.rotate_vector(tilt);

            //triangles.push_back(Triangle(a, b, c, col, specular, reflective));
            shapes.push_back(new Triangle(a, b, c, col, specular, reflective));

        }
    }

    //shapes.push_back(new Plane(triangles));

    shapes_number = shapes.size();

    lights[0] = new Ambient_light(0.2);
    lights[1] = new Point_light(0.6, Point(2, 1, 0));
    lights[2] = new Directional_light(0.2, Vector(1, 4, 4));
    lights_number = 3;
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

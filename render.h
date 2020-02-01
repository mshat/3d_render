#ifndef RENDER_H
#define RENDER_H

#include <QWidget>
#include "types.h"
#include "painter.h"
#include "canvas.h"
#include "ui_render.h"
#include "QPainter"
#include "QColor"
#include "color.h"
#include <iostream>
//#include "render.h"
#include "scene.h"
#include "limits.h"
#include "float.h"
#include "cmath"

using namespace std;

namespace Ui{
    class Render;
}

class Render : public QWidget
{
    Q_OBJECT

public:
    explicit Render(QWidget *parent = nullptr);
    ~Render();
    error show();

private:
    Ui::Render *ui;

    error make_render();
    error closest_intersection(double &closest_t, int &closest_sphere_i, Shape **shapes, int shapes_number, Point origin, Vector direction, double t_min, double t_max);
    error trace_ray(Color &color, Shape **shapes, int shapes_number, Light **lights, int lights_number, Point origin, Vector direction, double t_min, double t_max, int depth=4);
    error canvas_to_viewport(double &Vx, double &Vy, int x, int y, fov_t fov, size_int size);
    error intersect_ray_sphere(double &t1, double &t2, Sphere sphere, const Point O, Vector direction);
    error compute_lighting(double &intensity, Point P, Vector N, Shape **shapes, int shapes_number,Light **lights, int lights_number, int specular, Vector V);
    Vector reflect_ray(Vector v1, Vector v2);

    Canvas *canvas1;
    Painter *painter1;
    Scene *scene;
};

#endif // RENDER_H

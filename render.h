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
    error trace_ray(Color &color, Shape **shapes, int shapes_number, Light **lights, int lights_number, Vector D);
    error canvas_to_viewport(double &Vx, double &Vy, int x, int y, fov_t fov, size_int size);
    error intersect_ray_sphere(double &t1, double &t2, Sphere sphere, const Point O, Vector D);
    error compute_lighting(double &intensity, Point P, Vector N, Light **lights, int lights_number);

    Canvas *canvas1;
    Painter *painter1;
    Scene *scene;
};

#endif // RENDER_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QPainter"
#include "QColor"
#include "QTime"
#include <iostream>
#include "limits.h"
#include "float.h"
#include "cmath"

#include "types.h"
#include "painter.h"
#include "scene.h"
#include "canvas.h"
#include "color.h"

#include <stdexcept>
#include <stdlib.h>
#include <thread>

#define THREADS 8

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    error show();
    error show_render();
    void set_lighting(bool param) {this->lighting = param; }
    void set_diffuse(bool param) {this->diffuse = param; }
    void set_mirror(bool param) {this->mirror = param; }
    void set_shadow(bool param) {this->shadow = param; }
    void set_reflection(bool param) {this->reflection = param; }
    void set_multithreads(bool param) {this->multithreads = param; }
    void set_movable_object(Point &point) {this->movable_object = &point; }
    void spin_around_center(int dir_coeff);
    void move_object();
    void move_object(int x, int y, int z);
    void move_object(int x, int y, int z, double step);
    void rotate_camera(int pitch, int yaw, int roll);
    void rotate_camera(int pitch, int yaw, int roll, double step_deg);
    void init_lables();
    double tests[10000];
    int tests_n = 0;
    int scene_param[6] = {1, 1, 1, 1, 1, 1};

private slots:
    void on_pitch_button_clicked();

    void on_yaw_button_clicked();

    void on_roll_button_clicked();

    void on_leftMoveButton_clicked();

    void on_upMoveButton_clicked();

    void on_rightMoveButton_clicked();

    void on_downMoveButton_clicked();

    void on_aheadMoveButton_clicked();

    void on_backMoveButto_clicked();

    void on_lightingCheckBox_stateChanged(int arg1);

    void on_diffuseCheckBox_stateChanged(int arg1);

    void on_mirrorCheckBox_stateChanged(int arg1);

    void on_shadowCheckBox_stateChanged(int arg1);

    void on_multithreadsCheckBox_stateChanged(int arg1);

    void on_renderPushButton_clicked();

    void on_timeTestPushButton_clicked();

    void on_reflectionCheckBox_stateChanged(int arg1);

    void on_cameraRadioButton_clicked();

    void on_pointLightRadioButton_clicked();

    void on_directionLightRadioButton_clicked();

    void on_setPointIntensityPushButton_clicked();

    void on_setDirectionIntensityPushButton_clicked();

    void on_setAmbientIntensityPushButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_timeTestPushButton_4_clicked();

    void on_timeTestPushButton_2_clicked();

    void on_timeTestPushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;

    error make_render();
    error calculate_render(Painter &painter, borders brds);
    error closest_intersection(double &closest_t, int &closest_sphere_i, std::vector<Shape *> &shapes, int shapes_number, Point origin, Vector direction, double t_min, double t_max);
    error trace_ray(Color &color, std::vector<Shape *> &shapes, int shapes_number, Light **lights, int lights_number, Point origin, Vector direction, double t_min, double t_max, int depth=4);
    error canvas_to_viewport(double &Vx, double &Vy, int x, int y, fov_t fov, size_int size);
    error intersect_ray_sphere(double &t1, double &t2, Sphere sphere, const Point O, Vector direction);
    double intersect_ray_triangle(Triangle triangle, const Point O, Vector direction);
    error compute_lighting(double &intensity, Point P, Vector N, std::vector<Shape *> &shapes, int shapes_number, Light **lights, int lights_number, int specular, Vector V);
    Vector reflect_ray(Vector v1, Vector v2);

    Canvas *canvases[THREADS];
    Painter *painters[THREADS];
    Scene *scene;

    Point *movable_object;

    bool lighting = 1;
    bool diffuse = 1;
    bool mirror = 1;
    bool shadow = 1;
    bool reflection = 1;
    bool multithreads = 1;

};

#endif // WIDGET_H

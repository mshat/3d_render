#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    size_int one_cnavas_size(600);

    for(int i=0; i < THREADS; i++)
    {
        canvases[i] = new Canvas(one_cnavas_size);
        painters[i] = new Painter(*canvases[i]);
    }

    scene = new Scene();

    set_movable_object(scene->get_camera()->get_position());

    QPalette Pal(palette());

    // устанавливаем цвет фона
    Pal.setColor(QPalette::Background, Qt::black);
    ui->frame->setAutoFillBackground(true);
    ui->frame->setPalette(Pal);
    //ui->frame->show();

    QPixmap coord_sys = QPixmap("D:\\me\\paint\\1.png");
    QPixmap coord_sys1 = QPixmap("D:\\git\\3d_render\\cs.png");
    //ui->label_7->setPixmap(coord_sys);
}

Widget::~Widget()
{
    delete ui;

    for(int i=0; i < THREADS; i++)
    {
        delete canvases[i];
        delete painters[i];
    }
    delete scene;
    delete ui;
}

error Widget::show()
{
    error rc = NO;

    QWidget::show();

    init_lables();

    //rc = show_render();

    return rc;
}

error Widget::show_render()
{
    error rc = NO;

    //
    //    set_lighting(0);
    //    set_mirror(0);
    //    set_shadow(0);
    //    set_diffuse(0);
    //    set_reflection(0);
    //

    rc = make_render();

    ui->output_canvas1->setPixmap(canvases[0]->get_pixmap());
    ui->output_canvas2->setPixmap(canvases[1]->get_pixmap());
    ui->output_canvas3->setPixmap(canvases[2]->get_pixmap());
    ui->output_canvas4->setPixmap(canvases[3]->get_pixmap());
    ui->output_canvas5->setPixmap(canvases[4]->get_pixmap());
    ui->output_canvas6->setPixmap(canvases[5]->get_pixmap());
    ui->output_canvas7->setPixmap(canvases[6]->get_pixmap());
    ui->output_canvas8->setPixmap(canvases[7]->get_pixmap());

    return rc;
}

error Widget::make_render()
{
    error rc = NO;

    size_int sizes[THREADS];
    for(int i=0; i < THREADS; i++)
    {
        sizes[i] = painters[i]->get_canvas().get_size();
    }

    std::vector<std::thread> threads;
    borders brds[THREADS];
    for(int i=0; i < THREADS; i++)
    {
        brds[i] = borders(-300 + 75 * i, -300 + 75 * i + 75, sizes[i].height/2, -sizes[i].height/2);

        if (multithreads)
        {
            threads.push_back(std::thread(&Widget::calculate_render, this, std::ref(*painters[i]), brds[i]));
        }
        else
        {
            rc = calculate_render(*painters[i], brds[i]);
        }
    }

    for(int i=0; i < THREADS && multithreads; i++)
    {
        if (threads.at(i).joinable())
            threads.at(i).join();
    }

    return rc;
}

error Widget::calculate_render(Painter &painter, borders brds)
{
    error rc = NO;

    for (int x = brds.left; x < brds.right; x ++)
        for (int y = brds.bottom; (!rc) & (y < brds.top); y++)
        {
            double Vx, Vy;

            rc = canvas_to_viewport(Vx, Vy, x, y, scene->get_camera()->get_fov(), painter.get_canvas().get_size());

            Vector direction(Vx, Vy, scene->get_camera()->get_fov().d);
            direction.rotate_vector(scene->get_camera()->get_tilt());

            Color color;

            int depth_recursion = 3;

            if (!reflection)
            {
                depth_recursion = 0;
            }

            rc = trace_ray(
                        color,
                        scene->get_shapes(),
                        scene->get_shapes_number(),
                        scene->get_lights(),
                        scene->get_lights_number(),
                        scene->get_camera()->get_position(),
                        direction,
                        1,
                        100,
                        depth_recursion
                        );

            painter.set_pixel(Canvas_point(x, y), Painter_color(color));
        }

    return rc;
}

error Widget::trace_ray(
        Color &color,
        std::vector<Shape *> &shapes,
        int shapes_number,
        Light **lights,
        int lights_number,
        Point origin,
        Vector direction,
        double t_min,
        double t_max,
        int depth
        )
{
    error rc = NO;

    double closest_t = INT_MAX;
    int closest_shape_i = -1;

    closest_intersection(closest_t, closest_shape_i, shapes, shapes_number, origin, direction, t_min, t_max);

    if (closest_shape_i == -1)
    {
        color = scene->base_color;
    }
    else
    {
        int type = shapes.at(closest_shape_i)->type;

        Point point = origin + (direction * closest_t); //вычисление пересечения
        Vector normal;

        if (type == SPHERE)
        {
            Sphere *closest_sphere = static_cast<Sphere *>(shapes.at(closest_shape_i));
            if (closest_sphere)
            {
                normal = point - shapes.at(closest_shape_i)->get_center();
            }
        }
        else if (type == PLANE)
        {
            Plane *closest_plane = static_cast<Plane *>(shapes[closest_shape_i]);
            if (closest_plane)
            {
                normal = closest_plane->get_normal();
            }
        }
        else if (type == TRIANGLE)
        {
            Triangle *closest_triangle = static_cast<Triangle *>(shapes[closest_shape_i]);
            if (closest_triangle)
            {
                Vector a = closest_triangle->get_b() - closest_triangle->get_a();
                Vector b = closest_triangle->get_c() - closest_triangle->get_a();

                normal = a.cross(b);
            }
        }

        normal = normal / normal.get_length();
        double intensity;
        compute_lighting(intensity, point, normal, shapes, shapes_number, lights, lights_number, shapes[closest_shape_i]->get_specular(), direction * -1);
        Color local_color = shapes[closest_shape_i]->get_color() * intensity;

        double r = shapes[closest_shape_i]->get_reflective();

        if (depth <= 0 || r <= 0)
        {
            color = local_color;
        }
        else
        {
            Vector ref_ray = reflect_ray((direction * -1), normal);
            Color ref_color;
            trace_ray(ref_color, shapes, shapes_number, lights, lights_number, point, ref_ray, EPSILON, DBL_MAX, depth-1);

            color = local_color * (1 - r) + ref_color * r;

            if (ref_color == scene->base_color)
            {
                color = local_color;
            }
        }
    }
    return rc;
}

error Widget::closest_intersection(
        double &closest_t,
        int &closest_shape_i,
        std::vector<Shape *> &shapes,
        int shapes_number,
        Point origin,
        Vector direction,
        double t_min,
        double t_max
        )
{
    error rc = NO;

    closest_t = INT_MAX;
    closest_shape_i = -1;

    for (int i = 0; i < shapes_number; ++i)
    {
        int type = shapes[i]->type;

        if (type == SPHERE)
        {
            Sphere *sphere = static_cast<Sphere *>(shapes[i]);
            if (sphere)
            {
                double t1, t2;
                rc = intersect_ray_sphere(t1, t2, *sphere, origin, direction);

                if (t1 > t_min && t1 < t_max && t1 < closest_t)
                {
                    closest_t = t1;
                    closest_shape_i = i;
                }
                if (t2 > t_min && t2 < t_max && t2 < closest_t)
                {
                    closest_t = t2;
                    closest_shape_i = i;
                }
            }
        }
        else if (type == PLANE)
        {
            Plane *plane = static_cast<Plane *>(shapes[i]);

            if (plane)
            {
                int rectangles_num = plane->get_rectangles_num();

                for (int j = 0; j < rectangles_num; j++)
                {
                    double t;

                    t = intersect_ray_triangle(plane->get_triangles().at(j), origin, direction);

                    if (t > t_min && t < t_max && t < closest_t)
                    {
                        closest_t = t;
                        closest_shape_i = i;
                    }
                }

            }
        }
        else if (type == TRIANGLE)
        {
            Triangle *triangle = static_cast<Triangle *>(shapes[i]);

            if (triangle)
            {
                double t;

                t = intersect_ray_triangle(*triangle, origin, direction);

                if (t > t_min && t < t_max && t < closest_t)
                {
                    closest_t = t;
                    closest_shape_i = i;
                }
            }
        }
        else if (type == RECTANGLE)
        {
            Rectangle *rectangle = static_cast<Rectangle *>(shapes[i]);

            if (rectangle)
            {
                double t;

                t = intersect_ray_triangle(rectangle->get_t1(), origin, direction);

                if (t > t_min && t < t_max && t < closest_t)
                {
                    closest_t = t;
                    closest_shape_i = i;
                    //rectangle->set_main_color(Color(0, 255, 0));
                }
                else
                {
                    t = intersect_ray_triangle(rectangle->get_t2(), origin, direction);

                    if (t > t_min && t < t_max && t < closest_t)
                    {
                        closest_t = t;
                        closest_shape_i = i;
                        //rectangle->set_main_color(Color(255, 0, 0));
                    }
                }
            }
        }
    }

    return rc;
}

//решает квадратное уравнение
error Widget::intersect_ray_sphere(double &t1, double &t2, Sphere sphere, const Point O, Vector direction)
{
    error rc = NO;

    Point C = sphere.get_center();
    double r = sphere.get_radius();
    Point OCp = O - C; //на самом деле это вектор, просто нет нужного конструктора
    Vector OC = OCp;

    double k1 = direction * direction;
    double k2 = 2 * (OC * direction);
    double k3 = OC * OC - r * r;

    double discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0)
    {
        t1 = DBL_MAX;
        t2 = DBL_MAX;
    }

    t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
    t2 = (-k2 - sqrt(discriminant)) / (2 * k1);

    return rc;
}

double Widget::intersect_ray_triangle(Triangle triangle, const Point O, Vector direction)
{
    double tmp, u, v;

    Vector e1 = triangle.get_e1();
    Vector e2 = triangle.get_e2();

    Vector p = direction.cross(e2);
    Vector T = O - triangle.get_a();
    Vector Q = T.cross(e1);

    tmp = p * e1;
    if (tmp > -EPSILON && tmp < EPSILON)
    {
        return  DBL_MAX;
    }

    tmp = 1.0 / tmp;
    u = tmp * (T * p);

    if (u < 0.0 || u > 1.0)
    {
        return  DBL_MAX;
    }

    v = tmp * (direction * Q);

    if (v < 0.0 || v > 1.0)
    {
        return  DBL_MAX;
    }

    if(u + v > 1.0)
    {
        return  DBL_MAX;
    }

    return tmp * (e2 * Q);

}

//пересчет координат из системы холста в систему окна просмотра
error Widget::canvas_to_viewport(double &Vx, double &Vy, int x, int y, fov_t fov, size_int size)
{
    error rc = NO;

    if (size.width * size.height == 0)
    {
        rc = ZERO_DIVIDE;
    }
    else
    {
        Vx = x * fov.width / size.width;
        Vy = y * fov.height / size.height;
    }

    return rc;
}

error Widget::compute_lighting(
        double &intensity,
        Point point,
        Vector normal,
        std::vector<Shape *> &shapes,
        int shapes_number,
        Light **lights,
        int lights_number,
        int specular,
        Vector V
        )
{
    error rc = NO;

    intensity = 0;

    for(int i = 0; i < lights_number; i++)
    {
        int type = lights[i]->type;

        if (type == AMBIENT)
        {
            Ambient_light *light = static_cast<Ambient_light *>(lights[i]);
            if (light)
            {
                intensity += lights[i]->get_intensity();
            }
        }
        else
        {
            Vector L;
            double t_max = DBL_MAX;

            if (type == POINT)
            {
                Point_light *light = static_cast<Point_light *>(lights[i]);
                if (light)
                {
                    L = light->get_position() - point;
                    t_max = 1.0;
                }
            }
            else // type == DIRECTIONAL
            {
                Directional_light *light = static_cast<Directional_light *>(lights[i]);
                if (light != nullptr)
                {
                    L = light->get_direction();
                    t_max = DBL_MAX;
                }
            }

            if (shadow)
            {
                double closest_t = INT_MAX;
                int shadow_shape_i = -1;

                closest_intersection(closest_t, shadow_shape_i, shapes, shapes_number, point, L, EPSILON, t_max);

                if (shadow_shape_i >= 0)
                    continue;
            }

            if (lighting)
            {
                if (diffuse)
                {
                    double N_L = normal * L;

                    if (N_L > 0)
                    {
                        intensity += lights[i]->get_intensity() * N_L /
                                (normal.get_length() * L.get_length());
                    }
                }

                if (mirror)
                {
                    if (specular != -1)
                    {
                        //Vector R = reflect_ray(L, normal);
                        Vector R = normal * ((normal * L) * 2) - L;
                        double R_V = R * V;
                        if (R_V > 0)
                        {
                            intensity += lights[i]->get_intensity() * pow(R_V/ (R.get_length() * V.get_length()), specular);
                        }
                    }
                }
            }
            else
                intensity = 1;
        }
    }
    return rc;
}

Vector Widget::reflect_ray(Vector v1, Vector v2)
{
    return v2 * ((v2 * v1) * 2) - v1;
}

void Widget::spin_around_center(int dir_coeff)
{
    tilt_t tilt = scene->get_camera()->get_tilt();

    double step = -0.1 * dir_coeff;
    if (abs(step - 0) > EPSILON)
    {
        tilt.yaw += step;

        scene->get_camera()->set_tilt(tilt);
        show_render();
    }

    Point pos = scene->get_camera()->get_position();

    step = 1 * dir_coeff;
    if (abs(step - 0) > EPSILON)
    {
        pos.set_x(pos.get_x() + step);
        pos.set_z(pos.get_z() + step / 3);

        scene->get_camera()->set_position(pos);
        show_render();
    }
}

void Widget::move_object()
{
    double step_x = 0.0, step_y = 0.0, step_z = 0.0;
    step_x = ui->moveStepDoubleSpinBox->value();
    step_y = ui->doubleSpinBox->value();
    step_z = ui->doubleSpinBox_2->value();

    movable_object->move_to(Point(step_x, step_y, step_z));

    init_lables();
}

void Widget::move_object(int x, int y, int z)
{
    double step = 0;
    step = ui->moveStepDoubleSpinBox->value();

    if (abs(step - 0) > EPSILON)
    {
        movable_object->move(step * x, step * y, step * z);

        //show_render();
    }

    init_lables();
}

void Widget::move_object(int x, int y, int z, double step)
{
    if (abs(step - 0) > EPSILON)
    {
        movable_object->move(step * x, step * y, step * z);

        //show_render();
    }

    init_lables();
}

void Widget::rotate_camera(int pitch, int yaw, int roll)
{
    double step = 0;
    step = ui->rotateStepDoubleSpinBox->value();
    step = convert_deg_to_rad(step);

    if (abs(step - 0) > EPSILON)
    {
        scene->get_camera()->rotate(step * pitch, step * yaw, step * roll);

        //show_render();
    }

    init_lables();
}

void Widget::rotate_camera(int pitch, int yaw, int roll, double step_deg)
{
    double step = 0;
    step = convert_deg_to_rad(step_deg);

    if (abs(step - 0) > EPSILON)
    {
        scene->get_camera()->rotate(step * pitch, step * yaw, step * roll);

        //show_render();
    }

    init_lables();
}

void Widget::on_pitch_button_clicked()
{
    rotate_camera(1, 0, 0);
}

void Widget::on_yaw_button_clicked()
{
    rotate_camera(0, 1, 0);
}

void Widget::on_roll_button_clicked()
{
    rotate_camera(0, 0, 1);
}

void Widget::on_leftMoveButton_clicked()
{
    move_object(-1, 0, 0);
}

void Widget::on_upMoveButton_clicked()
{
    move_object(0, 1, 0);
}

void Widget::on_rightMoveButton_clicked()
{
    move_object(1, 0, 0);
}

void Widget::on_downMoveButton_clicked()
{
    move_object(0, -1, 0);
}

void Widget::on_aheadMoveButton_clicked()
{
    move_object(0, 0, 1);
}

void Widget::on_backMoveButto_clicked()
{
    move_object(0, 0, -1);
}


void Widget::on_lightingCheckBox_stateChanged(int arg1)
{
    set_lighting(arg1);
    ui->diffuseCheckBox->setChecked(arg1);
    ui->mirrorCheckBox->setChecked(arg1);
    ui->diffuseCheckBox->setEnabled(arg1);
    ui->mirrorCheckBox->setEnabled(arg1);
    scene_param[0] = arg1;
}

void Widget::on_diffuseCheckBox_stateChanged(int arg1)
{
    set_diffuse(arg1);
    scene_param[1] = arg1;
}

void Widget::on_mirrorCheckBox_stateChanged(int arg1)
{
    set_mirror(arg1);
    scene_param[2] = arg1;
}

void Widget::on_shadowCheckBox_stateChanged(int arg1)
{
    set_shadow(arg1);
    scene_param[4] = arg1;
}

void Widget::on_multithreadsCheckBox_stateChanged(int arg1)
{
    set_multithreads(arg1);
    scene_param[5] = arg1;
}

void Widget::on_reflectionCheckBox_stateChanged(int arg1)
{
    set_reflection(arg1);
    scene_param[3] = arg1;
}

void Widget::on_renderPushButton_clicked()
{
    ui->frame_8->setGeometry(20, 50, 600, 600);
    repaint();
    show_render();
    ui->frame_8->setGeometry(20, 50, 0, 0);
    repaint();
}

void Widget::on_cameraRadioButton_clicked()
{
    set_movable_object(scene->get_camera()->get_position());
}

void Widget::on_pointLightRadioButton_clicked()
{
    Point_light *light = static_cast<Point_light *>(scene->get_lights()[1]);
    if (light)
    {
        set_movable_object(light->get_position());
    }
}

void Widget::on_directionLightRadioButton_clicked()
{
    Directional_light *light = static_cast<Directional_light *>(scene->get_lights()[2]);
    if (light)
    {
        set_movable_object(light->get_direction());
    }
}

void Widget::on_setPointIntensityPushButton_clicked()
{
    Light **lights = scene->get_lights();
    lights[1]->set_intensity(ui->intensityDoubleSpinBox->value());
    init_lables();
}

void Widget::on_setDirectionIntensityPushButton_clicked()
{
    Light **lights = scene->get_lights();
    lights[2]->set_intensity(ui->intensityDoubleSpinBox->value());
    init_lables();
}

void Widget::on_setAmbientIntensityPushButton_clicked()
{
    Light **lights = scene->get_lights();
    lights[0]->set_intensity(ui->intensityDoubleSpinBox->value());
    init_lables();
}

void Widget::init_lables()
{
    //cam tilt
    ui->label_14->setText(QString::number(convert_rad_to_deg(scene->get_camera()->get_tilt().pitch)));
    ui->label_16->setText(QString::number(convert_rad_to_deg(scene->get_camera()->get_tilt().yaw)));
    ui->label_18->setText(QString::number(convert_rad_to_deg(scene->get_camera()->get_tilt().roll)));

    //cam_pos
    ui->label_24->setText(QString::number(scene->get_camera()->get_position().get_x()));
    ui->label_21->setText(QString::number(scene->get_camera()->get_position().get_y()));
    ui->label_23->setText(QString::number(scene->get_camera()->get_position().get_z()));

    Light **lights = scene->get_lights();
    Point_light *point_light = static_cast<Point_light *>(lights[1]);
    Directional_light *directional_light = static_cast<Directional_light *>(lights[2]);
    Ambient_light *ambient_light = static_cast<Ambient_light *>(lights[0]);

    //point
    ui->label_43->setText(QString::number(point_light->get_position().get_x()));
    ui->label_49->setText(QString::number(point_light->get_position().get_y()));
    ui->label_46->setText(QString::number(point_light->get_position().get_z()));
    ui->label_27->setText(QString::number(point_light->get_intensity()));

    //dir
    ui->label_66->setText(QString::number(directional_light->get_direction().get_x()));
    ui->label_57->setText(QString::number(directional_light->get_direction().get_y()));
    ui->label_65->setText(QString::number(directional_light->get_direction().get_z()));
    ui->label_31->setText(QString::number(directional_light->get_intensity()));

    //Amb
    ui->label_33->setText(QString::number(ambient_light->get_intensity()));
}

void Widget::on_pushButton_clicked()
{
    set_movable_object(scene->get_camera()->get_position());
    ui->cameraRadioButton->setChecked(1);
    ui->pointLightRadioButton->setChecked(0);
    ui->directionLightRadioButton->setChecked(0);
    scene->get_camera()->to_standart();
    rotate_camera(1, 0, 0, 90);
    move_object(0, 0, 1, 2);
    move_object(0, 1, 0, 2);
}

void Widget::on_pushButton_2_clicked()
{
    set_movable_object(scene->get_camera()->get_position());
    ui->cameraRadioButton->setChecked(1);
    ui->pointLightRadioButton->setChecked(0);
    ui->directionLightRadioButton->setChecked(0);
    scene->get_camera()->to_standart();
    move_object(0, 0, 1, -2);
    move_object(0, 1, 0, -1.5);
}

void Widget::on_pushButton_3_clicked()
{
    set_movable_object(scene->get_camera()->get_position());
    ui->cameraRadioButton->setChecked(1);
    ui->pointLightRadioButton->setChecked(0);
    ui->directionLightRadioButton->setChecked(0);
    scene->get_camera()->to_standart();
    rotate_camera(0, 1, 0, -90);
    move_object(1, 0, 0, 5);
    move_object(0, 1, 0, -1.5);
    move_object(0, 0, 1, 2.5);
}

void Widget::on_pushButton_7_clicked()
{
    set_movable_object(scene->get_camera()->get_position());
    ui->cameraRadioButton->setChecked(1);
    ui->pointLightRadioButton->setChecked(0);
    ui->directionLightRadioButton->setChecked(0);
    scene->get_camera()->to_standart();
    rotate_camera(0, 1, 0, 90);
    move_object(1, 0, 0, -5);
    move_object(0, 1, 0, -1.5);
    move_object(0, 0, 1, 2.5);
}

void Widget::on_timeTestPushButton_4_clicked()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
    ui->listWidget_4->clear();
    ui->listWidget_5->clear();
    tests_n = 0;
}

void Widget::on_timeTestPushButton_clicked()
{
    QTime timer;
    int total_time = 0;

    timer.restart();

    on_pushButton_clicked();
    on_renderPushButton_clicked();
    repaint();
    on_pushButton_3_clicked();
    on_renderPushButton_clicked();
    repaint();
    on_pushButton_7_clicked();
    on_renderPushButton_clicked();
    repaint();
    on_pushButton_2_clicked();
    on_renderPushButton_clicked();
    repaint();

    total_time += timer.elapsed();

    tests[tests_n++] = total_time;
    ui->listWidget->addItem(QString::number(total_time));
    ui->listWidget_2->addItem(QString(scene_param[0] ? "+" : "-"));
    ui->listWidget_3->addItem(QString(scene_param[3] ? "+" : "-"));
    ui->listWidget_4->addItem(QString(scene_param[4] ? "+" : "-"));
    ui->listWidget_5->addItem(QString(scene_param[5] ? "+" : "-"));

}


void Widget::on_timeTestPushButton_2_clicked()
{
    int n = ui->spinBox->value();

    for (int i = 0; i < n; i++)
    {
        on_timeTestPushButton_clicked();
    }
}

void Widget::on_timeTestPushButton_3_clicked()
{
    int n = tests_n;
    double sr = 0;

    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            sr += tests[i];
        }

        sr = sr / n;
        ui->label_39->setText(QString::number(sr));
    }
}

void Widget::on_pushButton_4_clicked()
{
    move_object();
}

#include "render.h"

Render::Render(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Render)
{
    ui->setupUi(this);

    size_int render_size(500, 500);
    this->resize(render_size.width, render_size.height);
    ui->widget->resize(render_size.width, render_size.height);
    ui->output_canvas1->resize(render_size.width, render_size.height);


    canvas1 = new Canvas(render_size);
    painter1 = new Painter(*canvas1);
    scene = new Scene();
}

error Render::show()
{
    QWidget::show();

    error rc = NO;

    rc = make_render();

    ui->output_canvas1->setPixmap(canvas1->get_pixmap());

    return rc;
}

Render::~Render()
{
    delete painter1;
    delete canvas1;
    delete scene;
    delete ui;
}

error Render::make_render()
{
    error rc = NO;

    size_int size = painter1->get_canvas().get_size();

    borders brds(-size.width/2, size.width/2, size.height/2, -size.height/2);

    for (int x = brds.left; x < brds.right; x ++)
        for (int y = brds.bottom; (!rc) & (y < brds.top); y++)
        {
            double Vx, Vy;

            rc = canvas_to_viewport(Vx, Vy, x, y, scene->get_camera().get_fov(), painter1->get_canvas().get_size());

            Vector direction(Vx, Vy, scene->get_camera().get_fov().d);

            Color color;
            rc = trace_ray(
                        color,
                        scene->get_shapes(),
                        scene->get_shapes_number(),
                        scene->get_lights(),
                        scene->get_lights_number(),
                        direction
                        );

            painter1->set_pixel(Canvas_point(x, y), Painter_color(color));
        }
    return rc;
}

error Render::trace_ray(
        Color &color,
        Shape **shapes,
        int shapes_number,
        Light **lights,
        int lights_number,
        Vector D
        )
{
    error rc = NO;

    double closest_t = INT_MAX;
    Sphere closest_sphere;

    double t_min = 1;
    double t_max = INT_MAX;

    for (int i = 0; i < shapes_number; ++i)
    {
        int type = shapes[i]->type;

        if (type == SPHERE)
        {
            Sphere *sphere = static_cast<Sphere *>(shapes[i]);
            if (sphere)
            {
                double t1, t2;
                rc = intersect_ray_sphere(t1, t2, *sphere,scene->get_O(), D);

                if (t1 > t_min && t1 < t_max && t1 < closest_t)
                {
                    closest_t = t1;
                    closest_sphere = *sphere;
                }
                if (t2 > t_min && t2 < t_max && t2 < closest_t)
                {
                    closest_t = t1;
                    closest_sphere = *sphere;
                }
            }

            if (closest_sphere.get_color() == Color(255, 255, 255))
                color = Color(255, 255, 255);
            else
            {
                color = closest_sphere.get_color();

                Point P = closest_sphere.get_center() + D * closest_t; //вычисление пересечения
                Vector N = P - closest_sphere.get_center();
                N = N / N.get_length();
                double intensity;
                compute_lighting(intensity, P, N, lights, lights_number);
                color = closest_sphere.get_color() * intensity;
            }
        }
    }

    return rc;
}

//пересчет координат из системы холста в систему окна просмотра
error Render::canvas_to_viewport(double &Vx, double &Vy, int x, int y, fov_t fov, size_int size)
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

//решает квадратное уравнение
error Render::intersect_ray_sphere(double &t1, double &t2, Sphere sphere, const Point O, Vector D)
{
    error rc = NO;

    Point C = sphere.get_center();
    double r = sphere.get_radius();
    Point OCp = O - C;
    Vector OC = OCp;

    double k1 = D * D;
    double k2 = 2 * (OC * D);
    double k3 = OC * OC - r * r;

    double discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0);
    // return inf, inf

    t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
    t2 = (-k2 - sqrt(discriminant)) / (2 * k1);

    return rc;
}

error Render::compute_lighting(double &intensity, Point P, Vector N, Light **lights, int lights_number)
{
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
            if (type == POINT)
            {
                Point_light *light = static_cast<Point_light *>(lights[i]);
                if (light)
                {
                    L = light->get_position() - P;
                }
            }
            else if (type == DIRECTIONAL)
            {
                Directional_light *light = static_cast<Directional_light *>(lights[i]);
                if (light)
                {
                    L = light->get_direction();
                }
            }

            double N_L = N * L;
            if (N_L > 0)
            {
                intensity += lights[i]->get_intensity() * N_L;
                intensity /= (N.get_length() * L.get_length());
            }
        }
    }
}

#include "render.h"

#define LIGHTING 1
#define DIFFUSE 1
#define MIRROR 1

Render::Render(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Render)
{
    ui->setupUi(this);

    size_int render_size(600);
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

/*
ClosestIntersection(O, D, t_min, t_max) {
    closest_t = inf
    closest_sphere = NULL
    for sphere in scene.Spheres {
        t1, t2 = IntersectRaySphere(O, D, sphere)
        if t1 in [t_min, t_max] and t1 < closest_t
            closest_t = t1
            closest_sphere = sphere
        if t2 in [t_min, t_max] and t2 < closest_t
            closest_t = t2
            closest_sphere = sphere
    }
    return closest_sphere, closest_t
}
*/

error Render::trace_ray(
        Color &color,
        Shape **shapes,
        int shapes_number,
        Light **lights,
        int lights_number,
        Vector direction
        )
{
    error rc = NO;

    double closest_t = INT_MAX;
    Sphere *closest_sphere = nullptr;

    double t_min = 1;
    double t_max = DBL_MAX;

    for (int i = 0; i < shapes_number; ++i)
    {
        int type = shapes[i]->type;

        if (type == SPHERE)
        {
            Sphere *sphere = static_cast<Sphere *>(shapes[i]);
            if (sphere)
            {
                double t1, t2;
                rc = intersect_ray_sphere(t1, t2, *sphere, scene->get_O(), direction);

                if (t1 > t_min && t1 < t_max && t1 < closest_t)
                {
                    closest_t = t1;
                    closest_sphere = sphere;
                }
                if (t2 > t_min && t2 < t_max && t2 < closest_t)
                {
                    closest_t = t2;
                    closest_sphere = sphere;
                }
            }

            if (closest_sphere == nullptr)
            {
                color = scene->base_color;
            }
            else
            {
                Point point = scene->get_O() + (direction * closest_t); //вычисление пересечения
                Vector normal = point - closest_sphere->get_center();
                normal = normal / normal.get_length();
                double intensity;
                compute_lighting(intensity, point, normal, lights, lights_number, closest_sphere->get_specular(), direction * -1);
                color = closest_sphere->get_color() * intensity;
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
error Render::intersect_ray_sphere(double &t1, double &t2, Sphere sphere, const Point O, Vector direction)
{
    error rc = NO;

    Point C = sphere.get_center();
    double r = sphere.get_radius();
    Point OCp = O - C;
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

error Render::compute_lighting(
        double &intensity,
        Point point,
        Vector normal,
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
            if (type == POINT)
            {
                Point_light *light = static_cast<Point_light *>(lights[i]);
                if (light)
                {
                    L = light->get_position() - point;
                }
            }
            else // type == DIRECTIONAL
            {
                Directional_light *light = static_cast<Directional_light *>(lights[i]);
                if (light)
                {
                    L = light->get_direction();
                }
            }

            if (LIGHTING)
            {
                if (DIFFUSE)
                {
                    double N_L = normal * L;

                    if (N_L > 0)
                    {
                        intensity += lights[i]->get_intensity() * N_L /
                                (normal.get_length() * L.get_length());
                    }
                }

                if (MIRROR)
                {
                    if (specular != -1)
                    {
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

#include "render.h"

Render::Render(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Render)
{
    ui->setupUi(this);

    canvas1 = new Canvas(size_int(540, 370));
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

            rc = canvas_to_viewport(Vx, Vy, x, y, scene->get_camera().get_vof(), canvas1->get_size());

            Vector D(Vx, Vy, scene->get_camera().get_vof().d);

			Color color;
            rc = trace_ray(color, scene->sphere, D, x, y);
			
			painter1->set_pixel(Canvas_point(x, y), Painter_color(color));
        }
    return rc;
}

error Render::trace_ray(Color &color, Sphere &spheres, Vector D, int x, int y)
{
    error rc = NO;

    double closest_t = INT_MAX;
    Sphere closest_sphere;

    //for sphere in Spheres 
	//{
    //todo
		Sphere sphere = spheres;
    //todo

		double t1, t2;
		rc = intersect_ray_sphere(t1, t2, sphere,scene->get_O(), D);

		double t_min = 1;
		double t_max = INT_MAX;

		if (t1 > t_min && t1 < t_max && t1 < closest_t)
		{
			closest_t = t1;
			closest_sphere = sphere;
		}
		if (t2 > t_min && t2 < t_max && t2 < closest_t)
		{
				closest_t = t1;
				closest_sphere = sphere;
		}
	//}
	
	if (closest_sphere.get_color() == Color(255, 255, 255))
		color = Color(255, 255, 255);
	else
		color = sphere.get_color();
	
    return rc;
}

//пересчет координат из системы холста в систему окна просмотра
error Render::canvas_to_viewport(double &Vx, double &Vy, int x, int y, vof_t vof, size_int size)
{
    error rc = NO;

    if (size.width * size.height == 0)
    {
        rc = ZERO_DIVIDE;
    }
    else
    {
        Vx = x *vof.width / size.width;
        Vy = y * vof.height / size.height;
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

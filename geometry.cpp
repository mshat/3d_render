#include "geometry.h"

Shape::Shape()
{
    this->center = Point(0, 0, 0);
    this->color = Color(255, 255, 255);
}

Shape::Shape(Point center, Color color, int specular, double reflective)
{
    this->center = center;
    this->color = color;
    this->specular = specular;
    this->reflective = reflective;
}

const Point Shape::get_center() { return this->center; }

Color Shape::get_color() { return this->color; }

Sphere::Sphere() : Shape()
{
    this->radius = 0;
    this->type = SPHERE;
}

Sphere::Sphere(Point center, Color color, int specular, double reflective, double radius) : Shape(center, color, specular, reflective)
{
    this->radius = radius;
    this->type = SPHERE;
}

double Sphere::get_radius()
{
    return this->radius;
}

//Triangle
Triangle::Triangle() : Shape()
{
    this->a = Point(0, 0, 0);
    this->b = Point(0, 0, 0);
    this->c = Point(0, 0, 0);
    this->e1 = Vector(0, 0, 0);
    this->e2 = Vector(0, 0, 0);
    this->type = TRIANGLE;
}

Triangle::Triangle(Point a, Point b, Point c, Color color, int specular, double reflective) : Shape(Point(0, 0, 0), color, specular, reflective)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->e1 = b - a;
    this->e2 = c - a;
    this->type = TRIANGLE;
}

Triangle::Triangle(Vector a, Vector b, Vector c, Color color, int specular, double reflective) : Shape(Point(0, 0, 0), color, specular, reflective)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->e1 = b - a;
    this->e2 = c - a;
    this->type = TRIANGLE;
}

Vector Triangle::get_normal()
{
    Vector ab = b - a;
    Vector bc = c - a;

    return  ab.cross(bc);
}

//Rectangle

Rectangle::Rectangle() : Shape()
{
    this->a = Point(0, 0, 0);
    this->b = Point(0, 0, 0);
    this->c = Point(0, 0, 0);
    this->d = Point(0, 0, 0);
    this->t1 = Triangle();
    this->t2 = Triangle();
    this->color1 = Color(255, 255, 255);
    this->color2 = Color(255, 255, 255);
    this->type = RECTANGLE;
}

Rectangle::Rectangle(Point a, Point b, Point c, Point d, Color color1, Color color2, int specular, double reflective) : Shape(Point(0, 0, 0), color1, specular, reflective)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->color1 = color1;
    this->color2 = color2;
    this->t1 = Triangle(a, b, c, color1, specular, reflective);
    this->t2 = Triangle(b, c, d, color2, specular, reflective);
    this->type = RECTANGLE;
}

void Rectangle::set_main_color(int num)
{
    if (num == 1)
    {
        this->color = color1;
    }
    else if (num == 2)
    {
        this->color = color2;
    }
}

void Rectangle::set_main_color(Color clr)
{
    this->color = clr;
}

//Plane

Plane::Plane() : Shape()
{
    this->n = Vector(0, 0, 0);
    this->type = PLANE;
}

Plane::Plane(std::vector<Triangle> &triangles) : Shape(triangles.at(0).get_center(), triangles.at(0).get_color(), triangles.at(0).get_specular(), triangles.at(0).get_reflective())
{
    this->triangles = triangles;
    this->triangles_num = triangles.size();
    this->n = triangles.at(0).get_normal();
    this->type = PLANE;
}

//Point

Point::Point(double x, double y, double z)
{
    this->point.x = x;
    this->point.y = y;
    this->point.z = z;
}

Point::Point(const Point &other)
{
    this->point.x = other.point.x;
    this->point.y = other.point.y;
    this->point.z = other.point.z;
}

void Point::move(double step_x, double step_y, double step_z)
{

    point.x = point.x + step_x;
    point.y = point.y + step_y;
    point.z = point.z + step_z;

}

void Point::move_to(Point destination)
{
    this->point = destination.get_point();
}

double Point::get_x() const  { return point.x; }

double Point::get_y() const  { return point.y; }

double Point::get_z() const { return point.z; }

Point Point::operator-(const Point &other) const
{
    Point tmp = Point(point.x - other.get_x(),
                      point.y - other.get_y(),
                      point.z - other.get_z());
    return tmp;
}

Point Point::operator+(const Point &other) const
{
    Point tmp = Point(point.x + other.get_x(),
                      point.y + other.get_y(),
                      point.z + other.get_z());
    return tmp;
}

Point Point::operator*(double n) const
{
    Point tmp = Point(point.x * n,
                      point.y * n,
                      point.z * n);
    return tmp;
}

point_t Point::get_point()  { return point; }

void Point::set_x(double x) { this->point.x = x; }

void Point::set_y(double y) { this->point.y = y; }

void Point::set_z(double z) { this->point.z = z; }

Screen_point::Screen_point(Point canvas_point, size_int canvas_size) : Point()
{
    this->point.x = canvas_size.width / 2 + canvas_point.get_x();
    this->point.y = canvas_size.height / 2 - canvas_point.get_y();
    this->point.z = 0.0;
}

Canvas_point::Canvas_point(Point screen_point, size_int canvas_size) : Point()
{
    this->point.x = screen_point.get_x() - canvas_size.width / 2;
    this->point.y = canvas_size.height / 2 - screen_point.get_y();
    this->point.z = 0.0;
}

double Vector::get_length()
{
    return sqrt(pow(this->get_x(), 2) + pow(this->get_y(), 2) + pow(this->get_z(),2));
}

void Vector::rotate_vector(tilt_t tilt)
{
    double pitch = tilt.pitch; //тангаж (x)
    double yaw = tilt.yaw; //рыскание (y)
    double roll = tilt.roll; // крен (z)

    double matr_pitch[3][3] = {{1, 0, 0}, //x
                               {0, cos(pitch), -sin(pitch)},
                               {0, sin(pitch), cos(pitch)}};

    double matr_yaw[3][3] = {{cos(yaw), 0, sin(yaw)}, //y
                             {0, 1,       0},
                             {-sin(yaw), 0,  cos(yaw)}};

    double matr_roll[3][3] = {{cos(roll), -sin(roll), 0}, //z
                              {sin(roll), cos(roll), 0},
                              {0, 0, 1}};

    *this = *this * matr_pitch;
    *this = *this * matr_yaw;
    *this = *this * matr_roll;
}

double Vector::operator*(const Vector &other) const
{
    return this->get_x() * other.get_x() +
            this->get_y() * other.get_y() +
            this->get_z() * other.get_z();

}

double Vector::operator/(const Vector &other) const
{
    return this->get_x() * (1 / other.get_x()) +
            this->get_y() * (1 / other.get_y()) +
            this->get_z() * (1 / other.get_z());

}

Vector Vector::operator*(double num)
{
    return Vector(this->get_x() * num,
                  this->get_y() * num,
                  this->get_z() * num);
}

Vector Vector::operator/(double num)
{
    return Vector(this->get_x() / num,
                  this->get_y() / num,
                  this->get_z() / num);
}

Vector Vector::operator*(double matr[3][3])
{
    double res[3] = {0};
    double vec[3];
    vec[0] = this->get_x();
    vec[1] = this->get_y();
    vec[2] = this->get_z();

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        {
            res[i] += matr[i][j] * vec[j];
        }

    return Vector(res[0], res[1], res[2]);
}

Vector Vector::cross(Vector other)
{
    double t1 = this->get_y() * other.get_z() - this->get_z() * other.get_y();
    double t2 = this->get_x() * other.get_z() - this->get_z() * other.get_x();
    double t3 = this->get_x() * other.get_y() - this->get_y() * other.get_x();
    return Vector(t1, -t2, t3);
}


double convert_deg_to_rad(double deg)
{
    return deg * M_PI / 180;
}

double convert_rad_to_deg(double rad)
{
    return rad * 180 / M_PI;
}

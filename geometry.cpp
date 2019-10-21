#include "geometry.h"

Shape::Shape()
{

}

Shape::Shape(Point center, Color color)
{
    this->center = center;
    this->color = color;
}

const Point Shape::get_center() { return this->center; }

const Color Shape::get_color() { return this->color; }

Sphere::Sphere() : Shape()
{
    this->center = Point(0, 0, 0);
    this->color = Color(255, 255, 255);
    this->radius = 0;
}

Sphere::Sphere(Point center, Color color, double radius) : Shape(center, color)
{
    this->radius = radius;
}

double Sphere::get_radius()
{
    return this->radius;
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

double Vector::operator*(const Vector &other) const
{
    return this->get_x() * other.get_x() +
            this->get_y() * other.get_y() +
            this->get_z() * other.get_z();

}

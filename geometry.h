#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <vector>
#include "types.h"
#include "color.h"
#include "math.h"

#define SPHERE 1
#define TRIANGLE 2
#define RECTANGLE 3
#define PLANE 4

class Point
{
public:
    Point() {}
    Point(double x, double y, double z = 0.0);
    Point(const Point &other);

    void move(double step_x=0.0, double step_y=0.0, double step_z=0.0);

    double get_x() const;
    double get_y() const;
    double get_z() const;

    void set_x(double x);
    void set_y(double y);
    void set_z(double z);

    Point operator- (const Point &other) const;
    Point operator+ (const Point &other) const;
    Point operator* (double n) const;

    point_t get_point();

protected:
    point_t point;  
};

class Screen_point : public Point
{
public:
    Screen_point() : Point() {}
    Screen_point(double x, double y, double z = 0.0) : Point(x, y, z) {}
    Screen_point(const Screen_point &other) : Point(other) {}
    Screen_point(Point canvas_point, size_int canvas_size);

};


class Canvas_point : public Point
{
public:
    Canvas_point() : Point() {}
    Canvas_point(double x, double y) : Point(x, y) {}
    Canvas_point(const Canvas_point &other) : Point(other) {}
    Canvas_point(Point screen_point, size_int canvas_size);
};


class Vector : public Point
{
public:
    Vector() : Point() {}
    Vector(Point other): Point(other) {}
    Vector(double x, double y, double z) : Point(x, y, z) {}
    double get_length();

    void rotate_vector(tilt_t tilt);

    double operator* (const Vector &other) const; //скалярное произведение
    double operator/ (const Vector &other) const;
    Vector operator* (double num);
    Vector operator/ (double num);
    Vector operator* (double matr[3][3]);
    Vector cross (Vector other);
};

class Shape
{

public:
    Shape();
    Shape(Point center, Color color, int specular, double reflective);

    const Point get_center();
    Color get_color();
    int get_specular() {return specular;}
    double get_reflective() {return reflective; }
    int type = 0;

protected:
    Point center;
    Color color;
    int specular;
    double reflective;
};

class Sphere : public Shape
{
public:
    Sphere();
    Sphere(Point center, Color color, int specular, double reflective, double radius);

    double get_radius();

private:
    double radius;
};

class Triangle : public Shape
{
public:
    Triangle();
    Triangle(Point a, Point b, Point c, Color color, int specular, double reflective);
    Triangle(Vector a, Vector b, Vector c, Color color, int specular, double reflective);
    Point get_a() {return  a; }
    Point get_b() {return  b; }
    Point get_c() {return  c; }
    Vector get_e1() {return  e1; }
    Vector get_e2() {return  e2; }
    Vector get_normal();
protected:
    Point a, b, c;
    Vector e1, e2;
};

class Rectangle : public Shape
{
public:
    Rectangle();
    Rectangle(Point a, Point b, Point c, Point d, Color color1, Color color2, int specular, double reflective);
    Point get_d() {return d; }
    Triangle &get_t1() {return t1;}
    Triangle &get_t2() {return t2;}
    void set_main_color(int num);
    void set_main_color(Color color);

private:
    Triangle t1, t2;
    Color color1, color2;
    Point a, b, c, d;
};

class Plane : public Shape
{
public:
    Plane();
    Plane(std::vector<Triangle> &triangles);

    Vector get_normal() {return n;}
    int get_rectangles_num() {return triangles_num; }
    std::vector<Triangle> get_triangles() {return triangles; }

private:
    Vector n;
    int triangles_num;
    std::vector<Triangle> triangles;
};

#endif // GEOMETRY_H

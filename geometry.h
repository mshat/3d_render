#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include "types.h"
#include "color.h"
#include "math.h"

#define SPHERE 1

class Point
{
public:
    Point() {}
    Point(double x, double y, double z = 0.0);
    Point(const Point &other);

    double get_x() const;
    double get_y() const;
    double get_z() const;

    Point operator- (const Point &other) const;
    Point operator+ (const Point &other) const;

    point_t get_point();

protected:
    point_t point;

    void set_x(double x);
    void set_y(double y);
    void set_z(double z);
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

    double operator* (const Vector &other) const; //скалярное произведение
    Vector operator* (double num);
    Vector operator/ (double num);
};


class Shape
{

public:
    Shape();
    Shape(Point center, Color color, int specular);

    const Point get_center();
    Color get_color();
    int get_specular() {return specular;}
    int type = 0;

protected:
    Point center;
    Color color;
    int specular;

    //todo какие-то свойства для освещения
};

class Sphere : public Shape
{
public:
    Sphere();
    Sphere(Point center, Color color, int specular, double radius);

    double get_radius();

private:
    double radius;
};


//todo конкретные классы фигур, наследуются от базового
//плоскость (основная); шар; куб
//ОПРЕДЕЛИТЬСЯ КАК ВООБЩЕ ЗАДАЮТСЯ ФИГУРЫ
//СКОРЕЕ ВСЕГО НАДО ДОБАВИТЬ В БАЗОВОМ КЛАССЕ СВОЙСТВО ФУНКЦИЯ
//И ДЛЯ КАЖДОГО ОБЪЕКТА ЗАДАТЬ ЕГО ФУНКЦИЮ

#endif // GEOMETRY_H

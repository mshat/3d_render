#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include "types.h"
#include "geometry.h"

#define AMBIENT 0
#define POINT 1
#define DIRECTIONAL 2

class Light
{
public:
    Light(QObject *parent = nullptr);
    Light(double intensity);
    double get_intensity() {return intensity;}
    void set_intensity(double intensity) {this->intensity = intensity; }
    int type;

protected:
    double intensity;
};

class Ambient_light : public Light
{
public:
    Ambient_light(double intensity);

};

class Point_light : public Light
{
public:
    Point_light(double intensity, Point position);
    Point &get_position() {return position;}
private:
    Point position;
};

class Directional_light : public Light
{
public:
    Directional_light(double intensity, Vector direction);
    Vector &get_direction() {return  direction;}
private:
    Vector direction;
};

#endif // LIGHT_H

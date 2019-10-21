#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include "types.h"
#include "geometry.h"

class Light : public QObject
{
    Q_OBJECT
public:
    explicit Light(QObject *parent = nullptr);
    explicit Light(double intensity);

protected:
    double intensity;

signals:

public slots:
};

class Ambient_light : public Light
{
public:
    explicit Ambient_light(double intensity);

};

class Point_light : public Light
{
public:
    explicit Point_light(double intensity, Point position);

private:
    Point position;
};

class Directional_light : public Light
{
public:
    explicit Directional_light(double intensity, vector_t direction);

private:
    vector_t direction;
};

#endif // LIGHT_H

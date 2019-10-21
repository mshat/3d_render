#include "light.h"

Light::Light(QObject *parent) : QObject(parent)
{

}

Light::Light(double intensity)
{
    this->intensity = intensity;
}

Ambient_light::Ambient_light(double intensity) : Light(intensity) {}

Point_light::Point_light(double intensity, Point position) : Light(intensity)
{
    this->position = position;
}

Directional_light::Directional_light(double intensity, vector_t direction) : Light(intensity)
{
    this->direction = direction;
}

#include "light.h"

Light::Light(QObject *parent)
{

}

Light::Light(double intensity)
{
    this->intensity = intensity;
}

Ambient_light::Ambient_light(double intensity) : Light(intensity) {this->type = 'A';}

Point_light::Point_light(double intensity, Point position) : Light(intensity)
{
    this->position = position;
    this->type = 'P';
}

Directional_light::Directional_light(double intensity, Vector direction) : Light(intensity)
{
    this->direction = direction;
    this->type = 'D';
}

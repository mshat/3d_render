#include "color.h"

Color::Color(int r, int g, int b)
{
    this->rgb = rgb_t(r, g, b);
}

void Color::set_rgb(rgb_t &other)
{
    this->rgb = other;
}

void Color::set_rgb(int r, int g, int b)
{
    rgb_t tmp(r, g, b);

    set_rgb(tmp);
}

const Color Color::operator*(int k)
{
    Color tmp = Color(rgb.r * k, rgb.g * k, rgb.b * k);

    return tmp;
}

const Color Color::operator*(double num)
{
    Color tmp = Color(rgb.r * num, rgb.g * num, rgb.b * num);

    return tmp;
}

const Color Color::operator+(const Color &other)
{
    Color tmp = Color(rgb.r + other.rgb.r,
                      rgb.g + other.rgb.g,
                      rgb.b + other.rgb.b);

    return tmp;
}

const Color Color::operator*=(int k)
{
    set_rgb(rgb.r * k,
            rgb.g * k,
            rgb.b * k);

    return *this;
}

const Color Color::operator+=(const Color &other)
{
    set_rgb(rgb.r + other.rgb.r,
            rgb.g + other.rgb.g,
            rgb.b + other.rgb.b);

    return *this;
}

bool Color::operator==(const Color &other) const
{
    if (this->get_rgb().r == other.get_rgb().r &&
            this->get_rgb().g == other.get_rgb().g &&
            this->get_rgb().b == other.get_rgb().b)
        return true;
    else
        return false;
}

#ifndef COLOR_H
#define COLOR_H

#include <QObject>
#include "QColor"
#include "types.h"
#include <iostream>

using namespace std;

class Color
{
public:
    Color() {}
    Color(const int r, const int g, const int b);

    void set_rgb(rgb_t &other);
    void set_rgb(int r, int g, int b);
    const rgb_t get_rgb() const { return rgb; }

    const Color operator* (int k);
    const Color operator+ (const Color &other);
    const Color operator*= (int k);
    const Color operator+= (const Color &other);
    bool operator== (const Color &other) const;

private:
    rgb_t rgb;

};

class Painter_color : public QColor
{
public:
    Painter_color() : QColor() {}
    Painter_color(Color &color) :
        QColor(color.get_rgb().r, color.get_rgb().g, color.get_rgb().b) {}
};

#endif // COLOR_H

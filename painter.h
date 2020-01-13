#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include "QPainter"
#include "types.h"
#include "geometry.h"
#include "canvas.h"

class Painter
{

public:
    Painter();
    Painter(Canvas &canvas);

    void set_canvas(Canvas &canvas);
    error set_pixel(Canvas_point point, QColor color = Qt::black);
    const Canvas &get_canvas();

private:
    QPainter painter;
    QPen pen;
    Canvas *canvas;

    //todo использовать класс Цвет
};

#endif // PAINTER_H

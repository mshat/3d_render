#include "painter.h"

Painter::Painter()
{

}

Painter::Painter(Canvas &canvas)
{
    set_canvas(canvas);
    pen = QPen(Qt::black, 1);
    painter.setPen(pen);
}

void Painter::set_canvas(Canvas &canvas)
{
    this->canvas = &canvas;
    painter.begin(&canvas.get_pixmap());
    size_int temp_s = canvas.get_size();
    painter.eraseRect(0, 0, temp_s.width, temp_s.height);

    //todo функция для очистки канваса. Хранить массив
    //не фоновых пикселей и зкарашивать их поточечно для очистки
}

error Painter::set_pixel(Canvas_point point, QColor color)
{
    error rc = NO;
    size_int size = canvas->get_size();
    Screen_point point_for_draw(point, size);

    if (point_for_draw.get_point().x < size.width &&
            point_for_draw.get_point().y < size.height &&
            point_for_draw.get_point().x >= 0 &&
            point_for_draw.get_point().y >= 0)
    {
        pen.setColor(color);
        painter.setPen(pen);

        painter.drawPoint(point_for_draw.get_x(), point_for_draw.get_y());
    }
    else
    {
        rc = COORDS_OUTSIDE_CANVAS;
    }

    return rc;
}

const Canvas &Painter::get_canvas()
{
    return *(this->canvas);
}


#include "canvas.h"

Canvas::Canvas(size_int size)
{
    this->size = size;
    this->canvas = QPixmap(size.width, size.height);
}

//Canvas &Canvas::operator=(const Canvas &other)
//{
//    this->canvas = other.canvas;
//    this->size = other.size;
//}

size_int Canvas::get_size() const
{
    return this->size;
}

QPixmap &Canvas::get_pixmap()
{
    return this->canvas;
}

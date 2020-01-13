#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include "QPixmap"
#include "types.h"

class Canvas
{

public:
    Canvas() {}
    Canvas(size_int size);

    size_int get_size() const;
    QPixmap &get_pixmap();

private:
    QPixmap canvas;
    size_int size;

};

#endif // CANVAS_H

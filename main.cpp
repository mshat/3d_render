#include <QApplication>
#include "render.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Render w;
    w.show();

    return a.exec();
}

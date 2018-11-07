#include "kasa.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    kasa w;
    w.show();

    return app.exec();
}


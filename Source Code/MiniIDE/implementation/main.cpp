#include "miniide.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MiniIDE w;
    w.show();
    return a.exec();
}

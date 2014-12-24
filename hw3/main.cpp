#include <QApplication>
#include <QLabel>

#ifndef QT_NO_OPENGL
#include "MainWidget.h"

#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("cube");
    a.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWidget w;
    w.show();
#else
    QLabel * notifyLabel = new QLabel("OpenGL Support required");
    notifyLabel->show();
#endif
    return a.exec();
}
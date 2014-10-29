#include <QApplication>
#include <QDesktopWidget>

#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget window;
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();
    return a.exec();
}

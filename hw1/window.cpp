#include <QBoxLayout>
#include "window.h"
#include "glwidget.h"

Window::Window(QWidget *parent) :
    QWidget(parent)
{
    glWidget = new GLWidget;

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    setLayout(mainLayout);
}

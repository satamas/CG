#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGL>
#include <QGLShader>

class Widget : public QGLWidget
{
public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    Q_OBJECT
    QGLShaderProgram shader;
};

#endif // WIDGET_H

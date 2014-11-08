#include "widget.h"

Widget::Widget(QWidget *parent)
    : QGLWidget(parent),
      shader()
{

    QFileInfo vsh("1.vsh");
    if(vsh.exists()){
        QGLShader vertexShader(QGLShader::Vertex);
        if(vertexShader.compileSourceFile("1.vsh")){
            qDebug() << shader.log();
            close();
        }
    }

    QFileInfo fsh("1.fsh");
    if(fsh.exists()){
        if(!shader.addShaderFromSourceFile(QGLShader::Fragment, "1.fsh")){
            qDebug() << shader.log();
            close();
        }
    }
}

Widget::~Widget()
{

}

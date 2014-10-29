#include "glwidget.h"
#include <QtOpenGL>
#include <QFileInfo>
#include <QGLBuffer>
#include <QGLFunctions>
#include <math.h>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    timer(this)
{
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(rotate()));
    timer.setSingleShot(false);

    renderText(0, 0, "F1-default shader");
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(500, 500);
}

void GLWidget::initializeGL(){
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    projection.perspective(30.0f, 1.0f, 0.1f, 100.f);
    view.lookAt(QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    defaultShaderProg = new QGLShaderProgram();
    displayShaderProg = new QGLShaderProgram();
    nonDisplayShaderProg = new QGLShaderProgram();

    loadShaderProgram(defaultShaderProg, "1");
    loadShaderProgram(displayShaderProg, "2");
    loadShaderProgram(nonDisplayShaderProg, "3");
    currentShaderProg = defaultShaderProg;
    currentShaderProg->bind();

    triangleVAO.create();
    timer.start(rotationPeriod * rotatlionStep/360);
}

void GLWidget::loadShaderProgram(QGLShaderProgram * program, QString name){
    QFileInfo vshFile(name + ".vsh");
    if(vshFile.exists()){
        if(!program->addShaderFromSourceFile(QGLShader::Vertex, name + ".vsh")){
            qDebug() << "Can't load vertex shader " + name +":"<< defaultShaderProg->log();
            close();
        }
    } else{
        qDebug() << "Can't find vsh file.";
        close();
    }

    QFileInfo fshFile(name + ".fsh");
    if(fshFile.exists()){
        if(!program->addShaderFromSourceFile(QGLShader::Fragment, name + ".fsh")){
            qDebug() << "Can't load fragment shader " + name +":"<< defaultShaderProg->log();
            close();
        }
    } else{
        qDebug() << "Can't find fsh file.";
        close();
    }

    if(!program->link()){
        qDebug() << program->log();
        close();
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_F1){
        currentShaderProg = defaultShaderProg;
        currentShaderProg->bind();
    } else if(event->key()== Qt::Key_F2){
        currentShaderProg = displayShaderProg;
        currentShaderProg->bind();
        currentShaderProg->setUniformValue("checkSize", displayShaderCheckSize);
    }else if(event->key()== Qt::Key_F3){
        currentShaderProg = nonDisplayShaderProg;
        currentShaderProg->bind();
        currentShaderProg->setUniformValue("checkSize", nonDisplayShaderCheckSize);
    }
}


void GLWidget::paintGL()
{
    glClearColor(0, 0, 0, 1.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentShaderProg->setUniformValue("mvp", projection * view * model);


    QGLBuffer buffer(QGLBuffer::VertexBuffer);
    float vertices[9] = {
        0.6f, (float) (-0.2 * sqrt(3)), 0,
        -0.6f, (float) (-0.2 * sqrt(3)), 0,
        0, (float) (0.4 * sqrt(3)), 0
    };


    triangleVAO.bind();
    buffer.create();
    buffer.setUsagePattern(QGLBuffer::StaticDraw);
    buffer.bind();
    buffer.allocate(vertices, 3 * 3 * sizeof(float));

    currentShaderProg->setAttributeBuffer( "vertex", GL_FLOAT, 0, 3 );
    currentShaderProg->enableAttributeArray( "vertex" );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    buffer.release();
    triangleVAO.release();
    currentShaderProg->disableAttributeArray( "vertex" );

    swapBuffers();
}

void GLWidget::resizeGL(int w, int h){
    glViewport( 0, 0, w, h);
}

void GLWidget::rotate(){
    model.rotate(rotatlionStep, QVector3D(0,0,1));
    updateGL();
}

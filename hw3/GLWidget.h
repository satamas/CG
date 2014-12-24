
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QtCore/qcoreevent.h>
#include <QtGui/qevent.h>
#include "geometry.h"

class QBasicTimer;
class QGLShaderProgram;

class GLWidget : public QGLWidget, protected QGLFunctions
{
Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    void onKeyPress(QKeyEvent * e);
    virtual ~GLWidget();

signals:

public slots:
    void setTextureFactor(double textureFactor);
    void setTextureFiltration(int textureFilter);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);



    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

private:
    QBasicTimer *timer;
    QGLShaderProgram *program;

    Geometry *sphereGeometry;
    Geometry *quadGeometry;
    Geometry *cylindereGeometry;
    Geometry *currentGeometry;

    GLuint texture;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;

};

#endif // MAINWIDGET_H

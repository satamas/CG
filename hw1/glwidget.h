#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShader>
#include <QTimer>
#include <QOpenGLVertexArrayObject>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
signals:

public slots:
    void rotate();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent * event) override;
private:
    float rotatlionStep = 2;
    int rotationPeriod = 5000;
    float displayShaderCheckSize = 0.6;
    float nonDisplayShaderCheckSize = 0.15;

    QMatrix4x4 projection;
    QMatrix4x4 view;
    QMatrix4x4 model;

    QTimer timer;


    QGLShaderProgram * currentShaderProg = nullptr;
    QGLShaderProgram * defaultShaderProg = nullptr;
    QGLShaderProgram * displayShaderProg = nullptr;
    QGLShaderProgram * nonDisplayShaderProg = nullptr;

    QOpenGLVertexArrayObject triangleVAO;

    void initializeVAO();
    inline void loadShaderProgram(QGLShaderProgram * program, QString name);
};

#endif // GLWIDGET_H

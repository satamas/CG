#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShader>
#include <QTimer>
#include <QOpenGLVertexArrayObject>
#include <QGLFunctions>

class GLWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void onKeyPress(QKeyEvent * event);
signals:

public slots:
    void setFrequency(int newFrequency);
    void setWaveVector(int new_wave_vector);
private slots:
    void redraw();
protected:
    void initializeGL() override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
private:
    void initializeBuffers();
    struct Face {
        int vtx_idx[3];
        int uv_idx[3];
        int norm_idx[3];
    };

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    float distance = 25.0f;
    float theta = 0;

    int mouseX;
    int mouseY;

    QGLShaderProgram * currentShaderProg = nullptr;
    QGLShaderProgram * defaultShaderProg = nullptr;
    QGLShaderProgram * waveShaderProg = nullptr;

    GLuint _vboID[2];

    QTimer updateTimer;
    int fps = 25;
    float timeFromStart = 0;
    bool showGrid = false;
    bool waves = false;

    std::vector<QVector3D> _triangles;
    std::vector<QVector3D> _norms;
    QVector3D _modelCenter;
    float _modelRadius;

    void loadShaderProgram(QGLShaderProgram * program, QString name);
    void loadObjFile(QString filename);
    void setWaveModelParameters();
};

#endif // GLWIDGET_H

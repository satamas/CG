#include "GLWidget.h"

#include "spheregeometry.h"
#include "quadgeometry.h"
#include "cylindergeometry.h"

#include <QBasicTimer>

GLWidget::GLWidget(QWidget *parent) :
        QGLWidget(parent),
        timer(new QBasicTimer),
        program(new QGLShaderProgram),
        sphereGeometry(new SphereGeometry(1, 12, 24)),
        cylindereGeometry(new CylinderGeometry(1, 3, 12, 24)),
        quadGeometry(new QuadGeometry(4, 4)) {
}

GLWidget::~GLWidget() {
    delete timer;
    timer = 0;
    delete program;
    program = 0;
    delete sphereGeometry;
    sphereGeometry = 0;
    delete quadGeometry;
    quadGeometry = 0;
    delete cylindereGeometry;
    cylindereGeometry = 0;
    deleteTexture(texture);
}

void GLWidget::mousePressEvent(QMouseEvent *e) {
    // Saving mouse press position
    mousePressPosition = QVector2D(e->pos());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->pos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void GLWidget::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);

    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01)
        angularSpeed = 0.0;
    else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Update scene
        updateGL();
    }
}

void GLWidget::initializeGL() {
    initializeGLFunctions();

    qglClearColor(Qt::black);

    qDebug() << "Initializing shaders...";
    initShaders();

    qDebug() << "Initializing textures...";
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    qDebug() << "Initializing geometries...";
    sphereGeometry->init();
    cylindereGeometry->init();
    quadGeometry->init();
    currentGeometry = quadGeometry;

    // using QBasicTimer because its faster that QTimer
    timer->start(12, this);
}

void GLWidget::initShaders() {
    // Overriding system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compiling vertex shader
    if (!program->addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compiling fragment shader
    if (!program->addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Linking shader pipeline
    if (!program->link())
        close();

    // Binding shader pipeline for use
    if (!program->bind())
        close();

    // Restore system locale
    setlocale(LC_ALL, "");
}

void GLWidget::initTextures() {
    // Loading cube.png to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    texture = bindTexture(QImage(":/brickwork-texture.jpg"));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
}


void GLWidget::setTextureFiltration(int textureFiltration) {
    if (!textureFiltration) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    updateGL();
}

void GLWidget::resizeGL(int w, int h) {
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = (qreal) w / ((qreal) h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void GLWidget::paintGL() {
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program->setUniformValue("mvp_matrix", projection * matrix);

    // Using texture unit 0 which contains cube.png
    program->setUniformValue("texture", 0);

    // Draw cube geometry
    currentGeometry->draw(program);
}

QSize GLWidget::sizeHint() const {
    return QSize(640, 480);
}

void GLWidget::setTextureFactor(double textureFactor) {
    quadGeometry->setTextureFactor(textureFactor);
    sphereGeometry->setTextureFactor(textureFactor);
    cylindereGeometry->setTextureFactor(textureFactor);
    updateGL();
}

void GLWidget::onKeyPress(QKeyEvent *e) {
    if (e->key() == Qt::Key_F1) {
        currentGeometry = quadGeometry;
    } else if (e->key() == Qt::Key_F2) {
        currentGeometry = cylindereGeometry;
    } else if (e->key() == Qt::Key_F3) {
        currentGeometry = sphereGeometry;
    }
    updateGL();
}

#include "glwidget.h"
#include <QtOpenGL>
#include <QFileInfo>
#include <QGLBuffer>
#include <QGLFunctions>
#include <math.h>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    updateTimer(this)
{
    QObject::connect(&updateTimer, SIGNAL(timeout()), this, SLOT(redraw()));
    updateTimer.setSingleShot(false);
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
    loadObjFile("model.obj");
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    projection.perspective(30.0f, 1.0f, 0.1f, 100.f);
    view.lookAt(QVector3D(0.0f, 0.0f, -25.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    defaultShaderProg = new QGLShaderProgram();
    waveShaderProg = new QGLShaderProgram();

    loadShaderProgram(defaultShaderProg, "1");
    defaultShaderProg->bind();
    defaultShaderProg->setUniformValue("color_from_norm", false);
    loadShaderProgram(waveShaderProg, "2");
    setWaveModelParameters();
    currentShaderProg = defaultShaderProg;
    currentShaderProg->bind();

    _vertexVAO.create();
}

void GLWidget::setWaveModelParameters(){
    QVector3D center;
    for(QVector3D vertex : _triangles){
        center += vertex;
    }
    center /= _triangles.size();

    for(QVector3D vertex : _triangles){
        _modelRadius = std::max(_modelRadius, (vertex - center).length());
    }

    waveShaderProg->bind();
    waveShaderProg->setUniformValue("frequency", (float) 0);
    waveShaderProg->setUniformValue("center", center);
    waveShaderProg->setUniformValue("wave_vector_length", (float) M_2_PI / _modelRadius);
}

void GLWidget::loadObjFile(QString filename) {
    QFile file(filename);
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> textureUV;
    std::vector<Face> faces;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Cannot open " << filename << endl;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList elements = line.split(QRegExp("\\s+"));
        if (elements[0] == "v") {
            vertices.push_back(QVector3D(elements[1].toFloat(), elements[2].toFloat(), elements[3].toFloat()));
        }else if(elements[0] == "vn"){
            normals.push_back(QVector3D(elements[1].toFloat(), elements[2].toFloat(), elements[3].toFloat()));
        } else if(elements[0] == "vt"){
            textureUV.push_back(QVector2D(elements[1].toFloat(), elements[2].toFloat()));
        }  else if (elements[0] == "f") {
            Face face;
            for(int i = 0 ; i < 3; ++i){
                QStringList numbers = elements[i+1].split("/");
                face.vtx_idx[i] = numbers[0].toInt();
                face.uv_idx[i] = numbers[1].toInt();
                face.norm_idx[i] = numbers[2].toInt();
            }
            faces.push_back(face);
        }
        else if (line[0] == '#') { /* ignoring this line */ }
        else { /* ignoring this line */ }
    }

    for (auto face : faces) {
        for (int i = 0; i < 3; ++i) {
            _triangles.push_back(vertices[face.vtx_idx[i] - 1]);
            _norms.push_back(normals[face.norm_idx[i] - 1]);
        }
    }

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

void GLWidget::onKeyPress(QKeyEvent *event){
    if(event->key() == Qt::Key_F1){
        updateTimer.stop();
        currentShaderProg = defaultShaderProg;
        currentShaderProg->bind();
        defaultShaderProg->setUniformValue("color_from_norm", false);
        updateGL();
    } else if(event->key()== Qt::Key_F2){
        showGrid = !showGrid;
        updateGL();
    } else if(event->key()==Qt::Key_F3){
        currentShaderProg = defaultShaderProg;
        currentShaderProg->bind();
        defaultShaderProg->setUniformValue("color_from_norm", true);
        updateGL();
    } else if(event->key()==Qt::Key_F4){
        currentShaderProg = waveShaderProg;
        currentShaderProg->bind();
        updateTimer.start(1000 / fps);
    }
}


void GLWidget::paintGL()
{
    glClearColor(0, 0, 0, 1.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentShaderProg->setUniformValue("mvp", projection * view * model);


    QGLBuffer buffer(QGLBuffer::VertexBuffer);


    _vertexVAO.bind();
    QGLBuffer vertexBuffer(QGLBuffer::VertexBuffer);
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(_triangles.data(), sizeof(QVector3D) * _triangles.size());
    currentShaderProg->setAttributeBuffer( "vertex", GL_FLOAT, 0, 3 );
    currentShaderProg->enableAttributeArray( "vertex" );

    _normalsVAO.bind();
    QGLBuffer normalsBuffer(QGLBuffer::VertexBuffer);
    normalsBuffer.create();
    normalsBuffer.bind();
    normalsBuffer.allocate(_triangles.data(), sizeof(QVector3D) * _triangles.size());
    currentShaderProg->setAttributeBuffer( "norm", GL_FLOAT, 0, 3 );
    currentShaderProg->enableAttributeArray( "norm" );

    currentShaderProg->setUniformValue("color", QVector3D(1,1,1));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, _triangles.size());

    if(showGrid){
        currentShaderProg->setUniformValue("color", QVector3D(0.5,0.5,0.5));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-2, -2);
        glDrawArrays(GL_TRIANGLES, 0, _triangles.size());
    }

    buffer.release();
    _vertexVAO.release();
    currentShaderProg->disableAttributeArray( "vertex" );

    swapBuffers();
}

void GLWidget::resizeGL(int w, int h){
    glViewport( 0, 0, w, h);
}


void GLWidget::initializeVAO(){
    std::vector<QVector3D> triangles;
    std::vector<QVector3D> norms;


    _vertexVAO.create();
    _vertexVAO.bind();
    QGLBuffer vertexBuffer(QGLBuffer::VertexBuffer);
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(triangles.data(), sizeof(QVector3D) * triangles.size());
}

void GLWidget::redraw(){
    timeFromStart += ((float) updateTimer.interval()) / 1000.0;
    waveShaderProg->setUniformValue("time_from_start", timeFromStart);
    updateGL();
}

void GLWidget::setFrequency(int new_frequency){
    waveShaderProg->bind();
    waveShaderProg->setUniformValue("frequency", (float) M_2_PI * new_frequency / 10);
}

void GLWidget::setWaveVector(int new_wave_vector){
    waveShaderProg->bind();
    waveShaderProg->setUniformValue("wave_vector_length", (float) (float) M_2_PI * new_wave_vector / _modelRadius);
}


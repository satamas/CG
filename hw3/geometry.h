#ifndef CYLINDEGEOMETRY_H
#define CYLINDEGEOMETRY_H

#include <QtOpenGL/QGLShaderProgram>
#include <QGLFunctions>

class Geometry : protected QGLFunctions {
public:
    virtual ~Geometry() {
    };

    virtual void init() = 0;

    virtual void draw(QGLShaderProgram *program) = 0;

    void setTextureFactor(double textureFactor) {
        _textureFactor = textureFactor;
        initGeometry();
    }

protected:
    double _textureFactor = 1;
    virtual void initGeometry() = 0;
    struct VertexData {
        QVector3D position;
        QVector2D texCoord;
    };
};

#endif // CYLINDEGEOMETRY_H

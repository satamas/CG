#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QtOpenGL/QGLShaderProgram>
#include "geometry.h"

class SphereGeometry : public Geometry {
public:
    SphereGeometry(float radius, unsigned int rings, unsigned int sectors);

    virtual ~SphereGeometry();

    void init() override;

    void draw(QGLShaderProgram *program) override;

protected:
    void initGeometry() override;

private:
    float _radius;
    unsigned int _rings;
    unsigned int _sectors;
    std::vector<VertexData> _vertices;
//    std::vector<GLfloat> _normals;
    std::vector<GLushort> _indices;
    std::vector<GLuint> _vboIds;
    static int const NUMBER_OF_VBO = 2;

};

#endif // GEOMETRYENGINE_H

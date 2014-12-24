#ifndef CYLINDERGEOMETRY_H
#define CYLINDERGEOMETRY_H

#include "geometry.h"
#include "circlegeometry.h"

class CylinderGeometry : public Geometry {
public:
    CylinderGeometry(float radius, float height, unsigned int rings, unsigned int sectors);

    void init() override;

    void draw(QGLShaderProgram *program) override;

protected:
    void initGeometry() override;

private:
    float _radius;
    float _height;
    unsigned int _rings;
    unsigned int _sectors;
    CircleGeometry topCircle;
    CircleGeometry bottomCircle;

    std::vector<GLuint> _vboIds;
    std::vector<VertexData> _vertices;
    std::vector<GLushort> _indices;
    static int const NUMBER_OF_VBO = 2;
};

#endif // CYLINDERGEOMETRY_H
